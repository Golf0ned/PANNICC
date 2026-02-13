#include "backend/codegen.h"

namespace backend {
    void CodeGenVisitor::visit(lir::Instruction *i) {}

    void CodeGenVisitor::visit(lir::Label *l) {}

    void CodeGenVisitor::visit(lir::InstructionMov *i) {}

    void CodeGenVisitor::visit(lir::InstructionPush *i) {}

    void CodeGenVisitor::visit(lir::InstructionPop *i) {}

    void CodeGenVisitor::visit(lir::InstructionConvert *i) {}

    void CodeGenVisitor::visit(lir::InstructionBinaryOp *i) {}

    void CodeGenVisitor::visit(lir::InstructionSpecialOp *i) {}

    void CodeGenVisitor::visit(lir::InstructionLea *i) {}

    void CodeGenVisitor::visit(lir::InstructionCmp *i) {}

    void CodeGenVisitor::visit(lir::InstructionJmp *i) {}

    void CodeGenVisitor::visit(lir::InstructionCJmp *i) {}

    void CodeGenVisitor::visit(lir::InstructionCall *i) {}

    void CodeGenVisitor::visit(lir::InstructionRet *i) {}

    void CodeGenVisitor::visit(lir::InstructionUnknown *i) {}

    std::string generateCode(lir::Program &lir) {
        CodeGenVisitor cgv;
        for (auto &f : lir.getFunctions()) {
            // TODO: generate preamble and postamble
        }

        // TODO: validation
        // if (!validate(lir))
        //     return "invalid lir";

        return lir.toString();
    }
} // namespace backend
