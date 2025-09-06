#pragma once

#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    class InstructionVisitor;

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
        InstructionLoad(Type type, Value *ptr);
        Value *getPtr();
        void accept(InstructionVisitor *visitor);

    private:
        Value *ptr;
    };

    class InstructionStore : public Instruction {
    public:
        InstructionStore(Value *value, Value *ptr);
        Value *getValue();
        Value *getPtr();
        void accept(InstructionVisitor *visitor);

    private:
        Value *value;
        Value *ptr;
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
        void accept(InstructionVisitor *visitor);

    private:
        Value *value;
    };

    class InstructionVisitor {
    public:
        virtual void visit(InstructionBinaryOp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionAlloca *i) = 0;
        virtual void visit(InstructionLoad *i) = 0;
        virtual void visit(InstructionStore *i) = 0;

        virtual void visit(TerminatorReturn *t) = 0;
    };
} // namespace middleend::mir
