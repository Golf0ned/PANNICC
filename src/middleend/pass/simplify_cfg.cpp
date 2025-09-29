#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "middleend/pass/simplify_cfg.h"

namespace middleend {
    void SimplifyCFG::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            bool changed = true;
            while (changed) {
                changed = false;
                std::vector<size_t> to_erase;
                auto &bbs = f.getBasicBlocks();
                // TODO: handle non-sequential BB storage order
                for (size_t ind = 0; ind < bbs.size(); ind++) {
                    auto bb = bbs[ind].get();

                    auto &preds = bb->getPredecessors();
                    auto &succs = bb->getSuccessors();

                    // Remove if no predecessors
                    if (preds.empty() && bb != f.getEntryBlock()) {
                        for (auto pred : preds)
                            std::erase(pred->getSuccessors(), bb);

                        for (auto succ : succs)
                            std::erase(succ->getPredecessors(), bb);

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }

                    // Merge if straight line connection
                    if (preds.size() == 1 &&
                        preds[0]->getSuccessors().size() == 1) {
                        auto &pred = preds[0];

                        auto &pred_insts = pred->getInstructions();
                        for (auto &i : bb->getInstructions()) {
                            assert(
                                !dynamic_cast<mir::InstructionPhi *>(i.get()));
                            pred_insts.push_back(std::move(i));
                        }

                        auto &pred_literals = pred->getLiterals();
                        for (auto &l : bb->getLiterals())
                            pred_literals.push_back(std::move(l));

                        for (auto succ : succs) {
                            auto &succ_preds = succ->getPredecessors();
                            std::replace(succ_preds.begin(), succ_preds.end(),
                                         bb, pred);

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

                        pred->getSuccessors().swap(succs);
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
                        auto &succ = succs[0];
                        auto &succ_preds = succ->getPredecessors();
                        std::unordered_set<mir::BasicBlock *> succ_preds_has(
                            succ_preds.begin(), succ_preds.end());

                        // TODO: handle this case properly: we can't join if
                        // succ has a phi since its value depends on previous
                        // block
                        auto &succ_insts = succ->getInstructions();
                        bool succ_has_phi = !succ_insts.empty() &&
                                            dynamic_cast<mir::InstructionPhi *>(
                                                succ_insts[0].get());
                        if (succ_has_phi)
                            continue;

                        for (auto pred : preds) {
                            // Replace succ preds
                            if (!succ_preds_has.contains(pred)) {
                                succ_preds.push_back(pred);
                                succ_preds_has.insert(pred);
                            }

                            // Replace pred succs
                            auto &pred_succs = pred->getSuccessors();
                            std::replace(pred_succs.begin(), pred_succs.end(),
                                         bb, succ);

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

                        if (bb == f.getEntryBlock())
                            f.setEntryBlock(succ);

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }
                }

                //
                // Commit changes
                //
                if (to_erase.size() == 0)
                    continue;

                std::vector<std::unique_ptr<mir::BasicBlock>> new_bbs;
                new_bbs.reserve(bbs.size() - to_erase.size());

                size_t to_erase_ind = 0;
                for (size_t ind = 0; ind < bbs.size(); ind++) {
                    auto &bb = bbs[ind];
                    if (to_erase_ind < to_erase.size() &&
                        ind == to_erase[to_erase_ind]) {
                        to_erase_ind++;
                        continue;
                    }
                    new_bbs.push_back(std::move(bb));
                }

                f.getBasicBlocks().swap(new_bbs);
            }
        }
    }
} // namespace middleend
