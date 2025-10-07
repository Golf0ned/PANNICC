#include <ranges>
#include <unordered_set>

#include "middleend/analysis/dominator_tree.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/transform/mem2reg.h"

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

            std::unordered_map<mir::BasicBlock *,
                               std::unordered_map<mir::InstructionAlloca *,
                                                  mir::InstructionPhi *>>
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
                        phis[bb][alloca] = phi.get();
                        bb->getInstructions().push_front(std::move(phi));

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

                for (auto [alloca, phi] : phis[bb]) {
                    reaching[alloca] = phi;
                    for (auto succ : bb->getSuccessors().getEdges()) {
                        auto &succ_phis = phis[succ];
                        if (succ_phis.contains(alloca))
                            succ_phis[alloca]->getPredecessors()[bb] = phi;
                    }
                }

                auto &instructions = bb->getInstructions();
                auto iter = instructions.begin();
                while (iter != instructions.end()) {
                    auto i = iter->get();

                    // Alloca: mark for drop
                    auto *alloca = dynamic_cast<mir::InstructionAlloca *>(i);
                    if (alloca) {
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
                        for (auto succ : bb->getSuccessors().getEdges()) {
                            auto &succ_phis = phis[succ];
                            if (succ_phis.contains(alloca))
                                succ_phis[alloca]->getPredecessors()[bb] =
                                    value;
                        }

                        to_drop.push_back(std::move(*iter));
                        iter = instructions.erase(iter);
                        continue;
                    }

                    // Load: replace uses with reaching def, delete
                    auto *load = dynamic_cast<mir::InstructionLoad *>(i);
                    if (load) {
                        ReplaceUseVisitor visitor(load,
                                                  reaching[load->getPtr()]);
                        for (auto &use : load->getUses())
                            use->accept(&visitor);

                        to_drop.push_back(std::move(*iter));
                        iter = instructions.erase(iter);
                        continue;
                    }

                    iter++;
                }

                visited.insert(bb);
                for (auto succ : bb->getSuccessors().getEdges()) {
                    if (visited.contains(succ))
                        continue;
                    worklist.push_back({succ, reaching});
                }
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

    ReplaceUseVisitor::ReplaceUseVisitor(mir::Value *old_value,
                                         mir::Value *new_value)
        : old_value(old_value), new_value(new_value) {}

    void ReplaceUseVisitor::visit(mir::InstructionBinaryOp *i) {
        if (i->getLeft() == old_value)
            i->setLeft(new_value);
        if (i->getRight() == old_value)
            i->setRight(new_value);
    }

    void ReplaceUseVisitor::visit(mir::InstructionCall *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionAlloca *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionLoad *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionStore *i) {
        if (i->getValue() == old_value)
            i->setValue(new_value);
    }

    void ReplaceUseVisitor::visit(mir::InstructionPhi *i) {
        for (auto &[bb, val] : i->getPredecessors()) {
            if (val == old_value)
                i->getPredecessors()[bb] = new_value;
        }
    }

    void ReplaceUseVisitor::visit(mir::TerminatorReturn *t) {
        if (t->getValue() == old_value) {
            t->setValue(new_value);
        }
    }

    void ReplaceUseVisitor::visit(mir::TerminatorBranch *t) {}

    void ReplaceUseVisitor::visit(mir::TerminatorCondBranch *t) {
        if (t->getCond() == old_value)
            t->setCond(new_value);
    }
} // namespace middleend
