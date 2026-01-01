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

                    auto &preds = bb->getPredecessors();
                    for (auto &pred : preds.getUniqueEdges()) {
                        // TODO: insert parallel copies in predecessors if none
                        // TODO: update parallel copies in predecessors
                    }
                }
            }
        }
    }
} // namespace middleend
