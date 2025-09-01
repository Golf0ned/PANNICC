#pragma once

#include <cstdint>

#include "middleend/mir/operator.h"
#include "middleend/mir/type.h"

// TODO: figure out this valueIsLiteral slop

namespace middleend::mir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor);
        virtual ~Instruction() = default;
    };

    class InstructionDeclaration : public Instruction {
    private:
        Type type;
        uint64_t variable;
    };

    class InstructionAssignValue : public Instruction {
    private:
        uint64_t variable;
        uint64_t value;
        bool valueIsLiteral;
    };

    class InstructionAssignBinaryOp : public Instruction {
    private:
        uint64_t variable;
        uint64_t left;
        bool leftIsLiteral;
        BinaryOp op;
        uint64_t right;
        bool rightIsLiteral;
    };

    class InstructionCall : public Instruction {
    private:
        uint64_t callee;
    };

    class InstructionCallAssign : public Instruction {
    private:
        uint64_t variable;
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
        uint64_t value;
        bool valueIsLiteral;
    };
} // namespace middleend::mir
