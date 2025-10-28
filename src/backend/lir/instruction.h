#pragma once

#include "backend/lir/data_size.h"
#include "backend/lir/operand.h"

namespace backend::lir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor) = 0;
        virtual ~Instruction() = default;
    };

    class Label : public Instruction {
    public:
        Label(std::string name);
        std::string getName();

    private:
        std::string name;
    };

    class InstructionMov : public Instruction {
    public:
        InstructionMov(DataSize size, Operand *dst, Operand *src);
        void accept(InstructionVisitor *visitor);

    private:
        DataSize size;
        Operand *dst;
        Operand *src;
    };

    class InstructionPush : public Instruction {
    public:
        InstructionPush(DataSize size, Operand *src);
        void accept(InstructionVisitor *visitor);

    private:
        DataSize size;
        Operand *src;
    };

    class InstructionPop : public Instruction {
    public:
        InstructionPop(DataSize size, Operand *dst);
        void accept(InstructionVisitor *visitor);

    private:
        DataSize size;
        Operand *dst;
    };

    // class InstructionNeg : public Instruction {};
    // class InstructionNot : public Instruction {};
    // class InstructionLea : public Instruction {};
    // class InstructionAdd : public Instruction {};
    // class InstructionSub : public Instruction {};
    // class InstructionIMul : public Instruction {};
    // class InstructionIDiv : public Instruction {};
    // class InstructionAnd : public Instruction {};
    // class InstructionOr : public Instruction {};
    // class InstructionXor : public Instruction {};
    // class InstructionSal : public Instruction {};
    // class InstructionSar : public Instruction {};
    // class InstructionCmp : public Instruction {};
    // class InstructionTest : public Instruction {};
    // class InstructionJmp : public Instruction {};
    // class InstructionJe : public Instruction {};
    // class InstructionJg : public Instruction {};
    class InstructionCall : public Instruction {
    public:
        InstructionCall(std::string label);
        void accept(InstructionVisitor *visitor);

    private:
        std::string label;
    };
    class InstructionRet : public Instruction {
    public:
        void accept(InstructionVisitor *visitor);
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(InstructionMov *i) = 0;
        virtual void visit(InstructionPush *i) = 0;
        virtual void visit(InstructionPop *i) = 0;
        // virtual void visit(InstructionNeg *i) = 0;
        // virtual void visit(InstructionNot *i) = 0;
        // virtual void visit(InstructionLea *i) = 0;
        // virtual void visit(InstructionAdd *i) = 0;
        // virtual void visit(InstructionSub *i) = 0;
        // virtual void visit(InstructionIMul *i) = 0;
        // virtual void visit(InstructionIDiv *i) = 0;
        // virtual void visit(InstructionAnd *i) = 0;
        // virtual void visit(InstructionOr *i) = 0;
        // virtual void visit(InstructionXor *i) = 0;
        // virtual void visit(InstructionSal *i) = 0;
        // virtual void visit(InstructionSar *i) = 0;
        // virtual void visit(InstructionCmp *i) = 0;
        // virtual void visit(InstructionTest *i) = 0;
        // virtual void visit(InstructionJmp *i) = 0;
        // virtual void visit(InstructionJe *i) = 0;
        // virtual void visit(InstructionJg *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionRet *i) = 0;
    };
} // namespace backend::lir
