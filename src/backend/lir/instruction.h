#pragma once

#include "backend/lir/condition_code.h"
#include "backend/lir/data_size.h"
#include "backend/lir/operand.h"
#include "middleend/mir/operator.h"

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
        void accept(InstructionVisitor *v) override;

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
        void setSrc(lir::Operand *new_src);
        void setDst(lir::Operand *new_dst);
        void accept(InstructionVisitor *v) override;

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
        void setSrc(lir::Operand *new_src);
        void accept(InstructionVisitor *v) override;

    private:
        DataSize size;
        Operand *src;
    };

    class InstructionPop : public Instruction {
    public:
        InstructionPop(DataSize size, Operand *dst);
        DataSize getSize();
        Operand *getDst();
        void setDst(lir::Operand *new_dst);
        void accept(InstructionVisitor *v) override;

    private:
        DataSize size;
        Operand *dst;
    };

    class InstructionConvert : public Instruction {
    public:
        InstructionConvert(DataSize from, DataSize to);
        DataSize getFrom();
        DataSize getTo();
        void accept(InstructionVisitor *v) override;

    private:
        DataSize from;
        DataSize to;
    };

    enum class BinaryOp {
        ADD,
        SUB,
        IMUL,
        IDIV,
        AND,
        OR,
        XOR,
        SHL,
        SAR,
    };

    BinaryOp fromMir(middleend::mir::BinaryOp op);

    std::string toString(BinaryOp op);

    // TODO: div, inc, dec, neg, not

    class InstructionBinaryOp : public Instruction {
    public:
        InstructionBinaryOp(BinaryOp op, DataSize size, Operand *src,
                            Operand *dst);
        BinaryOp getOp();
        DataSize getSize();
        Operand *getSrc();
        Operand *getDst();
        void setSrc(lir::Operand *new_src);
        void setDst(lir::Operand *new_dst);
        void accept(InstructionVisitor *v) override;

    private:
        BinaryOp op;
        DataSize size;
        Operand *src;
        Operand *dst;
    };

    class InstructionSpecialOp : public Instruction {
    public:
        InstructionSpecialOp(BinaryOp op, DataSize size, Operand *src);
        BinaryOp getOp();
        DataSize getSize();
        Operand *getSrc();
        void setSrc(lir::Operand *new_src);
        void accept(InstructionVisitor *v) override;

    private:
        BinaryOp op;
        DataSize size;
        Operand *src;
    };

    class InstructionLea : public Instruction {
    public:
        InstructionLea(DataSize size, Address *src, Operand *dst);
        DataSize getSize();
        Address *getSrc();
        Operand *getDst();
        void setSrc(lir::Address *new_src);
        void setDst(lir::Operand *new_dst);
        void accept(InstructionVisitor *v) override;

    private:
        DataSize size;
        Address *src;
        Operand *dst;
    };

    // class InstructionNeg : public Instruction {};
    // class InstructionNot : public Instruction {};
    // class InstructionTest : public Instruction {};

    class InstructionCmp : public Instruction {
    public:
        InstructionCmp(DataSize size, Operand *src_1, Operand *src_2);
        DataSize getSize();
        Operand *getSrc1();
        Operand *getSrc2();
        void setSrc1(lir::Operand *new_src_1);
        void setSrc2(lir::Operand *new_src_2);
        void accept(InstructionVisitor *v) override;

    private:
        DataSize size;
        Operand *src_1;
        Operand *src_2;
    };

    class InstructionJmp : public Instruction {
    public:
        InstructionJmp(std::string label);
        std::string getLabel();
        void accept(InstructionVisitor *v) override;

    private:
        std::string label;
    };

    class InstructionCJmp : public Instruction {
    public:
        InstructionCJmp(ConditionCode cmp, std::string label);
        ConditionCode getCmp();
        std::string getLabel();
        void accept(InstructionVisitor *v) override;

    private:
        ConditionCode cmp;
        std::string label;
    };

    class InstructionCall : public Instruction {
    public:
        InstructionCall(std::string label, std::vector<Operand *> new_args);
        std::string getLabel();
        const std::vector<Operand *> &getArgs();
        void setArgs(std::vector<Operand *> args);
        void accept(InstructionVisitor *v) override;

    private:
        std::string label;
        std::vector<Operand *> args;
    };

    class InstructionRet : public Instruction {
    public:
        void accept(InstructionVisitor *v) override;
    };

    // TODO: remove once tiling's implemented
    class InstructionUnknown : public Instruction {
    public:
        void accept(InstructionVisitor *v) override;
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(Label *l) = 0;
        virtual void visit(InstructionMov *i) = 0;
        virtual void visit(InstructionPush *i) = 0;
        virtual void visit(InstructionPop *i) = 0;
        virtual void visit(InstructionConvert *i) = 0;
        virtual void visit(InstructionBinaryOp *i) = 0;
        virtual void visit(InstructionSpecialOp *i) = 0;
        virtual void visit(InstructionLea *i) = 0;
        virtual void visit(InstructionCmp *i) = 0;
        virtual void visit(InstructionJmp *i) = 0;
        virtual void visit(InstructionCJmp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionRet *i) = 0;
        virtual void visit(InstructionUnknown *i) = 0;
    };
} // namespace backend::lir
