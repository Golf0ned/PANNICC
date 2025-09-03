#pragma once

#include <cstdint>

#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor);
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
        InstructionCall(Type type, uint64_t callee);
        uint64_t getCallee();
        void accept(InstructionVisitor *visitor);

    private:
        uint64_t callee;
    };

    class Terminator : public Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor);
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

        virtual void visit(TerminatorReturn *t) = 0;
    };
} // namespace middleend::mir
