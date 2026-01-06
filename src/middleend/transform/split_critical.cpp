#include "middleend/transform/split_critical.h"
#include "middleend/mir/instruction.h"

namespace middleend {
    void SplitCritical::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            auto &bbs = f->getBasicBlocks();
            for (auto bb_iter = bbs.begin(); bb_iter != bbs.end(); bb_iter++) {
                auto bb = bb_iter->get();

                auto &bb_preds = bb->getPredecessors();
                for (auto &pred : bb_preds.getUniqueEdges()) {
                    auto &pred_succs = pred->getSuccessors();
                    if (pred_succs.getUniqueEdges().size() < 2)
                        continue;

                    // Insert new block
                    std::list<std::unique_ptr<mir::Instruction>> body;
                    auto terminator =
                        std::make_unique<mir::TerminatorBranch>(bb);
                    auto new_block_unique = std::make_unique<mir::BasicBlock>(
                        std::move(body), std::move(terminator));
                    auto new_block = new_block_unique.get();
                    bbs.insert(bb_iter, std::move(new_block_unique));

                    // Update pred out edges + new block in edges
                    auto &new_preds = new_block->getPredecessors();
                    for (auto out_edge : pred_succs.getEdges())
                        if (out_edge == bb) {
                            pred_succs.removeEdge(bb);
                            pred_succs.addEdge(new_block);
                            new_preds.addEdge(pred);
                        }

                    // Update bb in edges + new block out edges
                    auto &new_succs = new_block->getSuccessors();
                    for (auto in_edge : bb_preds.getEdges())
                        if (in_edge == pred) {
                            bb_preds.removeEdge(pred);
                            bb_preds.addEdge(new_block);
                            new_succs.addEdge(bb);
                        }

                    // Update phis in bb
                    for (auto &i : bb->getInstructions()) {
                        auto phi = dynamic_cast<mir::InstructionPhi *>(i.get());
                        if (!phi)
                            break;

                        auto &phi_preds = phi->getPredecessors();
                        phi_preds[new_block] = phi_preds.at(pred);
                        phi_preds.erase(pred);
                    }

                    // Clean up pred terminator
                    auto pred_terminator = pred->getTerminator().get();

                    auto branch =
                        dynamic_cast<mir::TerminatorBranch *>(pred_terminator);
                    if (branch) {
                        branch->setSuccessor(new_block);
                        continue;
                    }

                    auto cond_branch =
                        dynamic_cast<mir::TerminatorCondBranch *>(
                            pred_terminator);
                    if (cond_branch) {
                        if (cond_branch->getTSuccessor() == bb)
                            cond_branch->setTSuccessor(new_block);
                        if (cond_branch->getFSuccessor() == bb)
                            cond_branch->setFSuccessor(new_block);
                    }
                }
            }
        }
    }
} // namespace middleend
