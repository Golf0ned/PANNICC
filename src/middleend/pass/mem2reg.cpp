#include <iostream>
#include <ranges>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/pass/dominator_tree.h"
#include "middleend/pass/mem2reg.h"

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
                while (cur != i_dom) {
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
                std::vector<std::unique_ptr<mir::InstructionPhi>>>
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
                        phis[bb].push_back(std::move(phi));

                        visited.insert(bb);
                        if (!def_blocks.contains(bb))
                            worklist.push_back(bb);
                    }
                }
            }

            for (auto &[bb, phi_vec] : phis) {
                for (auto &phi : phi_vec) {
                    bb->getInstructions().insert(bb->getInstructions().begin(),
                                                 std::move(phi));
                }
            }

            //
            // Variable renaming
            //
            // std::vector<std::unique_ptr<mir::Instruction>> toPromote;
            // std::unordered_map<mir::InstructionAlloca *, mir::Value *>
            // curVal;
            //
            // std::unordered_set<mir::BasicBlock *> visited;
            // std::deque<mir::BasicBlock *> worklist;
            // worklist.push_back(f.getEntryBlock());
            // bool isEntry = true;
            //
            // while (!worklist.empty()) {
            //     auto cur = worklist.back();
            //     worklist.pop_back();
            //
            //     // Visited before: only update phis
            //     if (visited.contains(cur)) {
            //         // TODO: update phis
            //         continue;
            //     }
            //
            //     visited.insert(cur);
            //
            //     // Not visited: update phis and rename in instructions
            //     auto &instructions = cur->getInstructions();
            //     for (int ind = 0, end = instructions.size(); ind < end;
            //     ind++) {
            //         auto &i = instructions[ind];
            //
            //         // phi: add value from previous basic block
            //         // TODO: update phis
            //
            //         // alloca: if promotable (lives in entry block),
            //         // steal from instructions
            //         auto *alloca =
            //             dynamic_cast<mir::InstructionAlloca *>(i.get());
            //         if (isEntry && alloca) {
            //             toPromote.push_back(std::move(i));
            //             instructions.erase(instructions.begin() + ind);
            //             ind--;
            //             end--;
            //             continue;
            //         }
            //
            //         // store: update current value mapping
            //         // remove store from instructions
            //         auto *store =
            //             dynamic_cast<mir::InstructionStore *>(i.get());
            //         if (store) {
            //             curVal[store->getPtr()] = store->getValue();
            //             instructions.erase(instructions.begin() + ind);
            //             ind--;
            //             end--;
            //             continue;
            //         }
            //
            //         // load: replace all uses of the load with current value
            //         // remove load from instructions
            //         auto *load = dynamic_cast<mir::InstructionLoad
            //         *>(i.get()); if (load) {
            //             ReplaceUseVisitor visitor(load,
            //             curVal[load->getPtr()]); for (auto &use :
            //             load->getUses()) {
            //                 use->accept(&visitor);
            //             }
            //             instructions.erase(instructions.begin() + ind);
            //             ind--;
            //             end--;
            //             continue;
            //         }
            //     }
            //
            //     for (auto &next : cur->getSuccessors()) {
            //         worklist.push_back(next);
            //     }
            //
            //     isEntry = false;
            // }
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
        for (auto &pair : i->getPredecessors()) {
            if (pair.second == old_value)
                i->getPredecessors()[pair.first] = new_value;
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
