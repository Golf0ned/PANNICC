#include <cassert>

#include "middleend/transform/simplify_cfg.h"

namespace middleend {
    void SimplifyCFG::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            bool changed = true;
            while (changed) {
                changed = false;
                std::vector<size_t> to_erase;
                auto &bbs = f->getBasicBlocks();
                // TODO: handle non-sequential BB storage order
                size_t ind = 0;
                for (auto &bb_ref : bbs) {
                    auto bb = bb_ref.get();

                    auto &preds = bb->getPredecessors();
                    auto &succs = bb->getSuccessors();

                    // Remove if no predecessors
                    if (!preds.getSize() && bb != f->getEntryBlock()) {
                        for (auto succ : succs.getUniqueEdges())
                            succ->getPredecessors().removeEdge(bb);

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }

                    // Merge if straight line connection
                    if (preds.getSize() == 1) {
                        auto pred = preds.getEdges()[0];
                        if (pred->getSuccessors().getSize() != 1)
                            continue;

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

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }

                    // Eliminate phis if only one successor

                    // Eliminate if only unconditional br
                    auto terminator = dynamic_cast<mir::TerminatorBranch *>(
                        bb->getTerminator().get());
                    if (terminator && bb->getInstructions().empty()) {
                        auto succ = terminator->getSuccessor();
                        auto &succ_preds = succ->getPredecessors();
                        auto succ_preds_has = succ_preds.getUniqueEdges();
                        // TODO: handle this case properly: we can't join if
                        // succ has a phi since its value depends on previous
                        // block
                        auto &succ_insts = succ->getInstructions();
                        bool succ_has_phi = !succ_insts.empty() &&
                                            dynamic_cast<mir::InstructionPhi *>(
                                                succ_insts.front().get());
                        if (succ_has_phi)
                            continue;

                        succ_preds.removeEdge(bb);
                        for (auto pred : preds.getUniqueEdges()) {
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

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }
                    ind++;
                }

                //
                // Commit changes
                //
                if (to_erase.size() == 0)
                    continue;

                std::list<std::unique_ptr<mir::BasicBlock>> new_bbs;

                size_t new_ind = 0, to_erase_ind = 0;
                for (auto &bb : bbs) {
                    if (to_erase_ind < to_erase.size() &&
                        new_ind == to_erase[to_erase_ind]) {
                        to_erase_ind++;
                        new_ind++;
                        continue;
                    }
                    new_bbs.push_back(std::move(bb));
                    new_ind++;
                }

                f->getBasicBlocks().swap(new_bbs);
            }
        }
    }
} // namespace middleend
