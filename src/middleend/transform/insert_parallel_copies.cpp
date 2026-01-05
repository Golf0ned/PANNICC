#include "middleend/transform/insert_parallel_copies.h"
#include "middleend/mir/instruction.h"

namespace middleend {
    void InsertParallelCopies::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            for (auto &bb : f->getBasicBlocks()) {
                for (auto &i : bb->getInstructions()) {
                    auto phi = dynamic_cast<mir::InstructionPhi *>(i.get());
                    if (!phi)
                        break;
                    auto &phi_preds = phi->getPredecessors();

                    auto &preds = bb->getPredecessors();
                    for (auto &pred : preds.getUniqueEdges()) {
                        auto &pred_instructions = pred->getInstructions();

                        // TODO: represent with attribute, not instruction
                        if (pred_instructions.empty() ||
                            !dynamic_cast<mir::InstructionParallelCopy *>(
                                pred_instructions.back().get())) {
                            auto parallel_copy = std::make_unique<
                                mir::InstructionParallelCopy>();
                            pred_instructions.push_back(
                                std::move(parallel_copy));
                        }

                        auto parallel_copy =
                            static_cast<mir::InstructionParallelCopy *>(
                                pred_instructions.back().get());

                        parallel_copy->setCopy(phi, phi_preds.at(pred));
                    }
                }
            }
        }
    }
} // namespace middleend
