#pragma once

#include <list>
#include <memory>

#include "backend/lir/instruction.h"

namespace backend::lir {
    class Program {
    public:
        Program(std::list<std::unique_ptr<Instruction>> instructions,
                std::unique_ptr<OperandManager> om);
        std::list<std::unique_ptr<Instruction>> &getInstructions();
        OperandManager *getOm();
        std::string toString();

    private:
        std::list<std::unique_ptr<Instruction>> instructions;
        std::unique_ptr<OperandManager> om;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        std::string getResult();

        void visit(Instruction *i) override;
        void visit(Label *l) override;
        void visit(InstructionMov *i) override;
        void visit(InstructionPush *i) override;
        void visit(InstructionPop *i) override;
        void visit(InstructionConvert *i) override;
        void visit(InstructionBinaryOp *i) override;
        void visit(InstructionSpecialOp *i) override;
        void visit(InstructionLea *i) override;
        void visit(InstructionCmp *i) override;
        void visit(InstructionJmp *i) override;
        void visit(InstructionCJmp *i) override;
        void visit(InstructionCall *i) override;
        void visit(InstructionRet *i) override;

        void visit(InstructionVirtualCall *i) override;
        void visit(InstructionUnknown *i) override;

    private:
        std::string result;
    };
} // namespace backend::lir
