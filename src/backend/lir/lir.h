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
        std::string toString();

    private:
        std::list<std::unique_ptr<Instruction>> instructions;
        std::unique_ptr<OperandManager> om;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        std::string getResult();

        virtual void visit(Instruction *i);
        virtual void visit(Label *l);
        virtual void visit(InstructionMov *i);
        virtual void visit(InstructionPush *i);
        virtual void visit(InstructionPop *i);
        virtual void visit(InstructionBinaryOp *i);
        virtual void visit(InstructionCmp *i);
        virtual void visit(InstructionJmp *i);
        virtual void visit(InstructionCJmp *i);
        virtual void visit(InstructionCall *i);
        virtual void visit(InstructionRet *i);

        virtual void visit(InstructionPhi *i);
        virtual void visit(InstructionVirtual *i);
        virtual void visit(InstructionUnknown *i);

    private:
        std::string result;
    };
} // namespace backend::lir
