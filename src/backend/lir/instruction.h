#pragma once

#include "backend/lir/condition_code.h"
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
        void accept(InstructionVisitor *visitor);

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
    // class InstructionTest : public Instruction {};

    class InstructionCmp : public Instruction {
    public:
        InstructionCmp(Operand *src_1, Operand *src_2);
        void accept(InstructionVisitor *visitor);

    private:
        Operand *src_1;
        Operand *src_2;
    };

    class InstructionJmp : public Instruction {
    public:
        InstructionJmp(std::string label);
        void accept(InstructionVisitor *visitor);

    private:
        std::string label;
    };

    class InstructionCJmp : public Instruction {
    public:
        InstructionCJmp(ConditionCode cmp, std::string label);
        void accept(InstructionVisitor *visitor);

    private:
        ConditionCode cmp;
        std::string label;
    };

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
        virtual void visit(InstructionCmp *i) = 0;
        virtual void visit(InstructionJmp *i) = 0;
        virtual void visit(InstructionCJmp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionRet *i) = 0;
    };
} // namespace backend::lir
