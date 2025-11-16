#include <cassert>

#include "middleend/transform/simplify_cfg.h"

namespace middleend {
    void SimplifyCFG::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            bool changed = true;
            while (changed) {
                changed = false;
                std::vector<std::unique_ptr<mir::BasicBlock>> to_drop;
                auto &bbs = f->getBasicBlocks();
                // TODO: handle non-sequential BB storage order
                auto iter = bbs.begin();
                while (iter != bbs.end()) {
                    auto bb = iter->get();

                    auto &preds = bb->getPredecessors();
                    auto &succs = bb->getSuccessors();

                    auto tryRemoveOrphaned = [&]() {
                        if (preds.getSize() || bb == f->getEntryBlock())
                            return false;

                        for (auto succ : succs.getEdges())
                            succ->getPredecessors().removeEdge(bb);

                        to_drop.push_back(std::move(*iter));
                        iter = bbs.erase(iter);
                        return true;
                    };

                    auto tryMergeStraightLine = [&]() {
                        if (preds.getSize() != 1)
                            return false;
                        auto pred = preds.getEdges()[0];
                        if (pred == f->getEntryBlock() ||
                            pred->getSuccessors().getSize() != 1)
                            return false;

                        auto &pred_insts = pred->getInstructions();
                        for (auto &i : bb->getInstructions()) {
                            assert(
                                !dynamic_cast<mir::InstructionPhi *>(i.get()));
                            pred_insts.push_back(std::move(i));
                        }

                        for (auto succ : succs.getEdges()) {
                            auto &succ_preds = succ->getPredecessors();
                            succ_preds.removeEdge(bb);
                            succ_preds.addEdge(pred);

                            for (auto &i : succ->getInstructions()) {
                                auto phi = dynamic_cast<mir::InstructionPhi *>(
                                    i.get());
                                if (!phi)
                                    break;
                                if (!phi->getPredecessors().contains(bb))
                                    continue;

                                auto pair = phi->getPredecessors().extract(bb);
                                phi->getPredecessors()[pred] = pair.mapped();
                            }
                        }

                        pred->getSuccessors() = std::move(succs);
                        pred->getTerminator().swap(bb->getTerminator());

                        to_drop.push_back(std::move(*iter));
                        iter = bbs.erase(iter);
                        return true;
                    };

                    auto tryMergeUncondBr = [&]() {
                        auto terminator = dynamic_cast<mir::TerminatorBranch *>(
                            bb->getTerminator().get());
                        if (!terminator || bb->getInstructions().size())
                            return false;
                        auto succ = terminator->getSuccessor();
                        auto &succ_preds = succ->getPredecessors();
                        auto succ_preds_has = succ_preds.getUniqueEdges();
                        // TODO: handle this case properly: we can't join if
                        // succ has a phi since its value depends on
                        // previous block
                        auto &succ_insts = succ->getInstructions();
                        bool succ_has_phi = !succ_insts.empty() &&
                                            dynamic_cast<mir::InstructionPhi *>(
                                                succ_insts.front().get());
                        if (succ_has_phi)
                            return false;

                        succ_preds.removeEdge(bb);
                        for (auto pred : preds.getEdges()) {
                            // Replace succ preds
                            succ_preds.addEdge(pred);

                            // Replace pred succs
                            auto &pred_succs = pred->getSuccessors();
                            pred_succs.removeEdge(bb);
                            pred_succs.addEdge(succ);

                            // Replace pred terminator blocks
                            auto pred_terminator = pred->getTerminator().get();

                            auto branch = dynamic_cast<mir::TerminatorBranch *>(
                                pred_terminator);
                            if (branch) {
                                branch->setSuccessor(succ);
                                continue;
                            }

                            auto cond_branch =
                                dynamic_cast<mir::TerminatorCondBranch *>(
                                    pred_terminator);
                            if (cond_branch) {
                                if (cond_branch->getTSuccessor() == bb)
                                    cond_branch->setTSuccessor(succ);
                                if (cond_branch->getFSuccessor() == bb)
                                    cond_branch->setFSuccessor(succ);
                            }
                        }

                        if (bb == f->getEntryBlock())
                            f->setEntryBlock(succ);

                        to_drop.push_back(std::move(*iter));
                        iter = bbs.erase(iter);
                        return true;
                    };

                    // TODO: eliminate phi if only one successor
                    auto tryResolvePhi = [&]() { return false; };

                    // clang-format off
                    if (false
                        || tryRemoveOrphaned()
                        || tryMergeStraightLine()
                        || tryMergeUncondBr()
                        || tryResolvePhi()
                    ) {
                        // clang-format on
                        changed = true;
                        continue;
                    }

                    iter++;
                }
            }
        }
    }
} // namespace middleend
