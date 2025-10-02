#include <ranges>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/pass/dominator_tree.h"
#include "middleend/pass/mem2reg.h"
#include "middleend/pass/utils/replace_uses.h"

namespace middleend {
    std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock *>>
    computeDominanceFrontiers(DominatorTree *dt, mir::Function &f) {
        std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock *>>
            res;

        for (auto &bb : f.getBasicBlocks()) {
            auto &preds = bb->getPredecessors();
            if (preds.size() < 2)
                continue;

            for (auto pred : preds) {
                auto bb_ptr = bb.get();
                auto cur = pred, i_dom = dt->getImmediateDominator(bb_ptr);
                while (cur != nullptr && cur != i_dom) {
                    res[cur].push_back(bb_ptr);
                    cur = dt->getImmediateDominator(cur);
                }
            }
        }

        return res;
    }

    void Mem2Reg::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            //
            // Phi Insertion
            //
            std::unordered_map<mir::BasicBlock *,
                               std::vector<mir::BasicBlock *>>
                dominance_frontiers = computeDominanceFrontiers(dt, f);

            std::unordered_map<mir::InstructionAlloca *,
                               std::unordered_set<mir::BasicBlock *>>
                defs;
            for (auto &bb : f.getBasicBlocks()) {
                for (auto &i : bb->getInstructions()) {
                    auto *store =
                        dynamic_cast<mir::InstructionStore *>(i.get());
                    if (store)
                        defs[store->getPtr()].insert(bb.get());
                }
            }

            std::unordered_map<
                mir::BasicBlock *,
                std::unordered_map<mir::InstructionAlloca *,
                                   std::unique_ptr<mir::InstructionPhi>>>
                phis;
            for (auto const &[alloca, def_blocks] : defs) {
                mir::Type type = alloca->getAllocType();
                std::vector<mir::BasicBlock *> worklist(def_blocks.begin(),
                                                        def_blocks.end());
                std::unordered_set<mir::BasicBlock *> visited;
                while (!worklist.empty()) {
                    auto &block = worklist.back();
                    worklist.pop_back();
                    for (auto &bb : dominance_frontiers[block]) {
                        if (visited.contains(bb))
                            continue;

                        auto phi = std::make_unique<mir::InstructionPhi>(type);
                        phis[bb][alloca] = std::move(phi);

                        visited.insert(bb);
                        if (!def_blocks.contains(bb))
                            worklist.push_back(bb);
                    }
                }
            }

            //
            // Variable renaming
            //
            std::vector<std::pair<
                mir::BasicBlock *,
                std::unordered_map<mir::InstructionAlloca *, mir::Value *>>>
                worklist = {{f.getEntryBlock(), {}}};
            std::unordered_set<mir::BasicBlock *> visited;
            std::unordered_map<mir::BasicBlock *, std::vector<size_t>>
                indices_to_erase;

            while (!worklist.empty()) {
                auto &bb = worklist.back().first;
                auto reaching = std::move(worklist.back().second);
                worklist.pop_back();

                for (auto &[alloca, phi] : phis[bb])
                    reaching[alloca] = phi.get();

                auto &instructions = bb->getInstructions();
                for (size_t ind = 0; ind < instructions.size(); ind++) {
                    auto i = instructions[ind].get();

                    // Alloca: delete
                    // TODO: delete when finding promotable
                    auto *alloca = dynamic_cast<mir::InstructionAlloca *>(i);
                    if (alloca) {
                        indices_to_erase[bb].push_back(ind);
                        continue;
                    }

                    // Store: update reaching def, delete
                    auto *store = dynamic_cast<mir::InstructionStore *>(i);
                    if (store) {
                        auto alloca = store->getPtr();
                        auto value = store->getValue();
                        reaching[alloca] = value;
                        for (auto succ : bb->getSuccessors()) {
                            auto &succ_phis = phis[succ];
                            if (succ_phis.contains(alloca))
                                succ_phis[alloca]->getPredecessors()[bb] =
                                    value;
                        }
                        indices_to_erase[bb].push_back(ind);
                        continue;
                    }

                    // Load: replace uses with reaching def, delete
                    auto *load = dynamic_cast<mir::InstructionLoad *>(i);
                    if (load) {
                        ReplaceUsesVisitor visitor(
                            load, reaching[load->getPtr()], bb);
                        for (auto &[use, _] : load->getUses()) {
                            use->accept(&visitor);
                        }
                        indices_to_erase[bb].push_back(ind);
                        continue;
                    }
                }

                visited.insert(bb);
                for (auto succ : bb->getSuccessors()) {
                    if (visited.contains(succ))
                        continue;
                    worklist.push_back({succ, reaching});
                }
            }

            //
            // Final basic block modifications
            //
            for (auto &bb : f.getBasicBlocks()) {
                auto &instructions = bb->getInstructions();
                auto &to_insert = phis[bb.get()];
                auto &to_erase = indices_to_erase[bb.get()];

                std::vector<std::unique_ptr<mir::Instruction>> new_body;
                new_body.reserve(instructions.size() + to_insert.size() -
                                 to_erase.size());

                for (auto &[alloca, phi] : phis[bb.get()])
                    new_body.push_back(std::move(phi));

                size_t to_erase_ind = 0;
                for (size_t ind = 0; ind < instructions.size(); ind++) {
                    if (to_erase_ind < to_erase.size() &&
                        ind == to_erase[to_erase_ind]) {
                        to_erase_ind++;
                        continue;
                    }
                    new_body.push_back(std::move(instructions[ind]));
                }

                bb->getInstructions().swap(new_body);
            }
        }
    }

    void Mem2Reg::registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) {
        for (auto &pass : analyses) {
            auto dt = dynamic_cast<DominatorTree *>(pass.get());
            if (dt) {
                this->dt = dt;
                required_analyses.push_back(dt);
            }
        }

        if (!this->dt) {
            auto dt = std::make_unique<DominatorTree>();
            this->dt = dt.get();
            required_analyses.push_back(dt.get());
            analyses.push_back(std::move(dt));
        }
    }
} // namespace middleend
