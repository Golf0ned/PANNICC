#pragma once

#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    class InstructionVisitor;
    class Function;
    class BasicBlock;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor) = 0;
        virtual ~Instruction() = default;
    };

    class InstructionBinaryOp : public Instruction, public Value {
    public:
        InstructionBinaryOp(Type type, BinaryOp op, Value *left, Value *right);
        BinaryOp getOp();
        Value *getLeft();
        Value *getRight();
        void setLeft(Value *newVal);
        void setRight(Value *newVal);
        void accept(InstructionVisitor *visitor);

    private:
        BinaryOp op;
        Value *left;
        Value *right;
    };

    class InstructionCall : public Instruction, public Value {
    public:
        InstructionCall(Type type, Function *callee);
        Function *getCallee();
        void accept(InstructionVisitor *visitor);

    private:
        Function *callee;
    };

    class InstructionAlloca : public Instruction, public Value {
    public:
        InstructionAlloca(Type allocType);
        Type getAllocType();
        void accept(InstructionVisitor *visitor);

    private:
        Type allocType;
    };

    class InstructionLoad : public Instruction, public Value {
    public:
        InstructionLoad(Type type, InstructionAlloca *ptr);
        InstructionAlloca *getPtr();
        void accept(InstructionVisitor *visitor);

    private:
        InstructionAlloca *ptr;
    };

    class InstructionStore : public Instruction {
    public:
        InstructionStore(Value *value, InstructionAlloca *ptr);
        Value *getValue();
        InstructionAlloca *getPtr();
        void setValue(Value *newVal);
        void accept(InstructionVisitor *visitor);

    private:
        Value *value;
        InstructionAlloca *ptr;
    };

    class InstructionPhi : public Instruction, public Value {
    public:
        void addPredecessor(BasicBlock *bb, Value *v);
        void accept(InstructionVisitor *visitor);

    private:
        std::unordered_map<BasicBlock *, Value *> predecessors;
    };

    class Terminator : public Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor) = 0;
        virtual ~Terminator() = default;
    };

    class TerminatorReturn : public Terminator {
    public:
        TerminatorReturn(Value *value);
        Value *getValue();
        void setValue(Value *newVal);
        void accept(InstructionVisitor *visitor);

    private:
        Value *value;
    };

    class TerminatorBranch : public Terminator {
    public:
        TerminatorBranch(BasicBlock *successor);
        void accept(InstructionVisitor *visitor);

    private:
        BasicBlock *successor;
    };

    class TerminatorCondBranch : public Terminator {
    public:
        TerminatorCondBranch(Value *cond, BasicBlock *t_successor,
                             BasicBlock *f_successor);
        void accept(InstructionVisitor *visitor);

    private:
        Value *cond;
        BasicBlock *t_successor;
        BasicBlock *f_successor;
    };

    class InstructionVisitor {
    public:
        virtual void visit(InstructionBinaryOp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionAlloca *i) = 0;
        virtual void visit(InstructionLoad *i) = 0;
        virtual void visit(InstructionStore *i) = 0;
        virtual void visit(InstructionPhi *i) = 0;

        virtual void visit(TerminatorReturn *t) = 0;
        virtual void visit(TerminatorBranch *t) = 0;
        virtual void visit(TerminatorCondBranch *t) = 0;
    };
} // namespace middleend::mir
