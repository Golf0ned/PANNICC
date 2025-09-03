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
        InstructionBinaryOp(BinaryOp op, Value *left, Value *right);
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
        InstructionCall(uint64_t callee);
        uint64_t getCallee();
        void accept(InstructionVisitor *visitor);

    private:
        uint64_t callee;
    };

    class Terminator : public Instruction, public Value {
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
} // namespace middleend::mir
