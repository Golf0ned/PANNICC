#pragma once

#include "backend/lir/lir.h"

namespace backend {
    class ReplaceStackArgVisitor : public lir::InstructionVisitor {
    public:
        ReplaceStackArgVisitor(uint64_t stack_bytes, lir::OperandManager *om);

        lir::Operand *tryReplace(lir::Operand *operand);

        void visit(lir::Instruction *i);
        void visit(lir::Label *l);
        void visit(lir::InstructionMov *i);
        void visit(lir::InstructionPush *i);
        void visit(lir::InstructionPop *i);
        void visit(lir::InstructionConvert *i);
        void visit(lir::InstructionBinaryOp *i);
        void visit(lir::InstructionSpecialOp *i);
        void visit(lir::InstructionLea *i);
        void visit(lir::InstructionCmp *i);
        void visit(lir::InstructionJmp *i);
        void visit(lir::InstructionCJmp *i);
        void visit(lir::InstructionCall *i);
        void visit(lir::InstructionRet *i);
        void visit(lir::InstructionUnknown *i);

    private:
        uint64_t stack_bytes;
        lir::OperandManager *om;
    };

    void legalize(lir::Program &lir);
} // namespace backend
