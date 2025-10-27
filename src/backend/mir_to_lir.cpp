#include "backend/mir_to_lir.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        // Generate trees
        // - make all the atomic trees per basic block
        // - make sure to add a node for copyToReg + copyFromReg
        // - preserve phis

        // Merge trees

        // Tile trees

        // Final cleanup
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionBinaryOp *i) {
        auto res = std::unique_ptr<RegisterNode>();
    }
} // namespace backend
