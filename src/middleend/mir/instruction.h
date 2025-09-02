#pragma once

#include <cstdint>

#include "middleend/mir/operator.h"
#include "middleend/mir/type.h"
#include "middleend/mir/value.h"

// TODO: figure out this valueIsLiteral slop

namespace middleend::mir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor);
        virtual ~Instruction() = default;
    };

    class InstructionAssignBinaryOp : public Instruction, public Value {
    private:
        BinaryOp op;
        Value left;
        Value right;
    };

    class InstructionCall : public Instruction {
    private:
        uint64_t callee;
    };

    class InstructionCallAssign : public Instruction, public Value {
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
        void accept(InstructionVisitor *visitor);
        ~TerminatorReturn();

    private:
        Value value;
    };
} // namespace middleend::mir
