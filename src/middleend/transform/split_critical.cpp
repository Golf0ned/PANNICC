#include "middleend/transform/split_critical.h"
#include "middleend/mir/instruction.h"

namespace middleend {
    void SplitCritical::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            auto &bbs = f->getBasicBlocks();
            for (auto bb_iter = bbs.begin(); bb_iter != bbs.end(); bb_iter++) {
                auto bb = bb_iter->get();

                auto &preds = bb->getPredecessors();
                for (auto &pred : preds.getUniqueEdges()) {
                    auto &pred_succ = pred->getSuccessors();
                    if (pred_succ.getUniqueEdges().size() < 2)
                        continue;

                    std::list<std::unique_ptr<mir::Instruction>> body;
                    auto terminator =
                        std::make_unique<mir::TerminatorBranch>(bb);
                    auto new_block = std::make_unique<mir::BasicBlock>(
                        std::move(body), std::move(terminator));

                    for (auto out_edge : pred_succ.getEdges())
                        if (out_edge == bb) {
                            pred_succ.removeEdge(bb);
                            pred_succ.addEdge(new_block.get());
                        }

                    for (auto in_edge : preds.getEdges())
                        if (in_edge == pred) {
                            preds.removeEdge(pred);
                            preds.addEdge(new_block.get());
                        }

                    for (auto &i : bb->getInstructions()) {
                        auto phi = dynamic_cast<mir::InstructionPhi *>(i.get());
                        if (!phi)
                            break;

                        auto &phi_preds = phi->getPredecessors();
                        phi_preds[new_block.get()] = phi_preds.at(pred);
                        phi_preds.erase(pred);
                    }

                    auto pred_terminator = pred->getTerminator().get();

                    auto branch =
                        dynamic_cast<mir::TerminatorBranch *>(pred_terminator);
                    if (branch) {
                        branch->setSuccessor(new_block.get());
                        continue;
                    }

                    auto cond_branch =
                        dynamic_cast<mir::TerminatorCondBranch *>(
                            pred_terminator);
                    if (cond_branch) {
                        if (cond_branch->getTSuccessor() == bb)
                            cond_branch->setTSuccessor(new_block.get());
                        if (cond_branch->getFSuccessor() == bb)
                            cond_branch->setFSuccessor(new_block.get());
                    }

                    bbs.insert(bb_iter, std::move(new_block));
                }
            }
        }
    }
} // namespace middleend
