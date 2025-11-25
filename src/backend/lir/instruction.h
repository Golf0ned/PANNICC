#pragma once

#include <list>

#include "backend/lir/condition_code.h"
#include "backend/lir/data_size.h"
#include "backend/lir/operand.h"

namespace backend::lir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *v) = 0;
        virtual ~Instruction() = default;
    };

    class Label : public Instruction {
    public:
        Label(std::string name);
        std::string getName();
        void accept(InstructionVisitor *v);

    private:
        std::string name;
    };

    class InstructionMov : public Instruction {
    public:
        InstructionMov(Extend extend, DataSize src_size, DataSize dst_size,
                       Operand *src, Operand *dst);
        Extend getExtend();
        DataSize getSrcSize();
        DataSize getDstSize();
        Operand *getSrc();
        Operand *getDst();
        void accept(InstructionVisitor *v);

    private:
        Extend extend;
        DataSize src_size;
        DataSize dst_size;
        Operand *src;
        Operand *dst;
    };

    class InstructionPush : public Instruction {
    public:
        InstructionPush(DataSize size, Operand *src);
        DataSize getSize();
        Operand *getSrc();
        void accept(InstructionVisitor *v);

    private:
        DataSize size;
        Operand *src;
    };

    class InstructionPop : public Instruction {
    public:
        InstructionPop(DataSize size, Operand *dst);
        DataSize getSize();
        Operand *getDst();
        void accept(InstructionVisitor *v);

    private:
        DataSize size;
        Operand *dst;
    };

    enum class BinaryOp {
        ADD,
        SUB,
    };

    std::string toString(BinaryOp op);

    class InstructionBinaryOp : public Instruction {
    public:
        InstructionBinaryOp(BinaryOp op, DataSize size, Operand *src,
                            Operand *dst);
        BinaryOp getOp();
        DataSize getSize();
        Operand *getSrc();
        Operand *getDst();
        void accept(InstructionVisitor *v);

    private:
        BinaryOp op;
        DataSize size;
        Operand *src;
        Operand *dst;
    };

    // class InstructionNeg : public Instruction {};
    // class InstructionNot : public Instruction {};
    // class InstructionLea : public Instruction {};
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
        InstructionCmp(DataSize size, Operand *src_1, Operand *src_2);
        DataSize getSize();
        Operand *getSrc1();
        Operand *getSrc2();
        void accept(InstructionVisitor *v);

    private:
        DataSize size;
        Operand *src_1;
        Operand *src_2;
    };

    class InstructionJmp : public Instruction {
    public:
        InstructionJmp(std::string label);
        std::string getLabel();
        void accept(InstructionVisitor *v);

    private:
        std::string label;
    };

    class InstructionCJmp : public Instruction {
    public:
        InstructionCJmp(ConditionCode cmp, std::string label);
        ConditionCode getCmp();
        std::string getLabel();
        void accept(InstructionVisitor *v);

    private:
        ConditionCode cmp;
        std::string label;
    };

    class InstructionCall : public Instruction {
    public:
        InstructionCall(std::string label);
        std::string getLabel();
        void accept(InstructionVisitor *v);

    private:
        std::string label;
    };

    class InstructionRet : public Instruction {
    public:
        void accept(InstructionVisitor *v);
    };

    class InstructionPhi : public Instruction {
    public:
        InstructionPhi(std::list<Operand *> src, Operand *dst);
        std::list<Operand *> &getSrc();
        Operand *getDst();
        void accept(InstructionVisitor *v);

    private:
        std::list<Operand *> src;
        Operand *dst;
    };

    class InstructionVirtual : public Instruction {
    public:
        InstructionVirtual(std::unique_ptr<Instruction> i);
        std::unique_ptr<Instruction> &getInstruction();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<Instruction> instruction;
    };

    // TODO: remove once tiling's implemented
    class InstructionUnknown : public Instruction {
    public:
        void accept(InstructionVisitor *v);
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(Label *l) = 0;
        virtual void visit(InstructionMov *i) = 0;
        virtual void visit(InstructionPush *i) = 0;
        virtual void visit(InstructionPop *i) = 0;
        virtual void visit(InstructionBinaryOp *i) = 0;
        virtual void visit(InstructionCmp *i) = 0;
        virtual void visit(InstructionJmp *i) = 0;
        virtual void visit(InstructionCJmp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionRet *i) = 0;

        virtual void visit(InstructionPhi *i) = 0;
        virtual void visit(InstructionVirtual *i) = 0;
        virtual void visit(InstructionUnknown *i) = 0;
    };
} // namespace backend::lir
