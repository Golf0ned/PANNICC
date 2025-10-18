#include <algorithm>
#include <map>
#include <ranges>
#include <unordered_set>

#include "middleend/analysis/dominator_tree.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/transform/mem2reg.h"
#include "middleend/utils/erase_uses.h"
#include "middleend/utils/replace_uses.h"

namespace middleend {
    std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock *>>
    computeDominanceFrontiers(DominatorTree *dt, mir::Function *f) {
        std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock *>>
            res;

        for (auto &bb : f->getBasicBlocks()) {
            auto &preds = bb->getPredecessors();
            if (preds.getEdges().size() < 2)
                continue;

            for (auto pred : preds.getEdges()) {
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
        EraseUsesVisitor erase;
        for (auto &f : p.getFunctions()) {
            //
            // Phi Insertion
            //
            std::unordered_map<mir::BasicBlock *,
                               std::vector<mir::BasicBlock *>>
                dominance_frontiers = computeDominanceFrontiers(dt, f.get());

            std::unordered_map<mir::InstructionAlloca *,
                               std::unordered_set<mir::BasicBlock *>>
                defs;
            for (auto &bb : f->getBasicBlocks()) {
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

                        phis[bb][alloca] =
                            std::make_unique<mir::InstructionPhi>(type);

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
                worklist = {{f->getEntryBlock(), {}}};
            std::unordered_set<mir::BasicBlock *> visited;
            std::vector<std::unique_ptr<mir::Instruction>> to_drop;
            while (!worklist.empty()) {
                auto &bb = worklist.back().first;
                auto reaching = std::move(worklist.back().second);
                worklist.pop_back();

                for (auto &[alloca, phi] : phis[bb]) {
                    reaching[alloca] = phi.get();
                    for (auto succ : bb->getSuccessors().getUniqueEdges()) {
                        auto &succ_phis = phis[succ];
                        if (succ_phis.contains(alloca))
                            succ_phis[alloca]->setPredecessor(bb, phi.get());
                    }
                }

                auto &instructions = bb->getInstructions();
                auto iter = instructions.begin();
                while (iter != instructions.end()) {
                    auto i = iter->get();

                    // Alloca: mark for drop
                    auto *alloca = dynamic_cast<mir::InstructionAlloca *>(i);
                    if (alloca) {
                        i->accept(&erase);
                        to_drop.push_back(std::move(*iter));
                        iter = instructions.erase(iter);
                        continue;
                    }

                    // Store: update reaching def, delete
                    auto *store = dynamic_cast<mir::InstructionStore *>(i);
                    if (store) {
                        auto alloca = store->getPtr();
                        auto value = store->getValue();
                        reaching[alloca] = value;
                        for (auto succ : bb->getSuccessors().getUniqueEdges()) {
                            auto &succ_phis = phis[succ];
                            if (succ_phis.contains(alloca))
                                succ_phis[alloca]->setPredecessor(bb, value);
                        }

                        i->accept(&erase);
                        to_drop.push_back(std::move(*iter));
                        iter = instructions.erase(iter);
                        continue;
                    }

                    // Load: replace uses with reaching def, delete
                    auto *load = dynamic_cast<mir::InstructionLoad *>(i);
                    if (load) {
                        ReplaceUsesVisitor visitor(load,
                                                   reaching[load->getPtr()]);
                        auto uses_range = std::views::keys(load->getUses());
                        std::vector<mir::Instruction *> uses(uses_range.begin(),
                                                             uses_range.end());
                        for (auto &use : uses)
                            use->accept(&visitor);

                        i->accept(&erase);
                        to_drop.push_back(std::move(*iter));
                        iter = instructions.erase(iter);
                        continue;
                    }

                    iter++;
                }

                visited.insert(bb);
                for (auto succ : bb->getSuccessors().getUniqueEdges()) {
                    if (visited.contains(succ))
                        continue;
                    worklist.push_back({succ, reaching});
                }
            }

            //
            // Finalize phis
            //
            auto cmp_phi = [&](std::unique_ptr<mir::InstructionPhi> &first,
                               std::unique_ptr<mir::InstructionPhi> &second) {
                auto first_preds = first->getPredecessors(),
                     second_preds = second->getPredecessors();
                auto first_size = first_preds.size(),
                     second_size = second_preds.size();

                if (first_size != second_size)
                    return first_size < second_size;

                std::map<uint64_t, uint64_t> first_map, second_map;
                for (auto [bb, val] : first_preds)
                    first_map[nir->getNumber(bb)] = nir->getNumber(val);
                for (auto [bb, val] : second_preds)
                    second_map[nir->getNumber(bb)] = nir->getNumber(val);

                auto first_iter = first_map.begin(),
                     second_iter = second_map.begin();
                while (first_iter != first_map.end()) {
                    if (*first_iter != *second_iter) {
                        auto [first_bb, first_val] = *first_iter;
                        auto [second_bb, second_val] = *second_iter;
                        return first_bb < second_bb ||
                               first_bb == second_bb && first_val < second_val;
                    }
                    first_iter++;
                    second_iter++;
                }

                return false;
            };
            for (auto &[bb, map] : phis) {
                std::vector<std::unique_ptr<mir::InstructionPhi>> to_sort;
                for (auto &[_, phi] : map) {
                    switch (phi->getPredecessors().size()) {
                    default:
                        to_sort.push_back(std::move(phi));
                        continue;
                    case 0: // empty phi: omit
                        continue;
                    case 1: // single branch phi: replace use with value
                        ReplaceUsesVisitor visitor(
                            phi.get(), phi->getPredecessors().begin()->second);
                        for (auto &[use, _] : phi->getUses())
                            use->accept(&visitor);
                        continue;
                    }
                }

                std::sort(to_sort.begin(), to_sort.end(), cmp_phi);
                for (auto &phi : to_sort)
                    bb->getInstructions().push_front(std::move(phi));
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
                continue;
            }

            auto nir = dynamic_cast<NumberIR *>(pass.get());
            if (nir) {
                this->nir = nir;
                required_analyses.push_back(nir);
            }
        }

        if (!this->dt) {
            auto dt = std::make_unique<DominatorTree>();
            this->dt = dt.get();
            required_analyses.push_back(dt.get());
            analyses.push_back(std::move(dt));
        }

        if (!this->nir) {
            auto nir = std::make_unique<NumberIR>();
            this->nir = nir.get();
            required_analyses.push_back(nir.get());
            analyses.push_back(std::move(nir));
        }
    }
} // namespace middleend
