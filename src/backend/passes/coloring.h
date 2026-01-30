#pragma once

#include "backend/lir/operand.h"
#include "backend/passes/interference.h"

namespace backend {
    using RegisterColoring =
        std::unordered_map<lir::Register *, lir::Register *>;

    class PrecoloringVisitor : public lir::InstructionVisitor {
    public:
        PrecoloringVisitor(lir::OperandManager *om);

        RegisterColoring getResult();

        void checkConstrained(lir::Operand *operand);

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
        void visit(lir::InstructionVirtualCall *i);
        void visit(lir::InstructionUnknown *i);

    private:
        lir::OperandManager *om;
        RegisterColoring precolorings;
    };

    class ColoringVisitor : public lir::InstructionVisitor {
    public:
        ColoringVisitor(const RegisterColoring &coloring,
                        lir::OperandManager *om);

        lir::Operand *tryColorOperand(lir::Operand *operand);

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
        void visit(lir::InstructionVirtualCall *i);
        void visit(lir::InstructionUnknown *i);

    private:
        const RegisterColoring &coloring;
        lir::OperandManager *om;
    };

    RegisterColoring getPrecoloring(lir::Program &lir);

    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference);

    void colorRegisters(lir::Program &lir, RegisterColoring &coloring);

    void printColoring(RegisterColoring &coloring);

} // namespace backend
