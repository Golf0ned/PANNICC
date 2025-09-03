#include <cstdint>

#include "middleend/mir/instruction.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    InstructionBinaryOp::InstructionBinaryOp(Type type, BinaryOp op,
                                             Value *left, Value *right)
        : Value(type), op(op), left(left), right(right) {}

    BinaryOp InstructionBinaryOp::getOp() { return op; }

    Value *InstructionBinaryOp::getLeft() { return left; }

    Value *InstructionBinaryOp::getRight() { return right; }

    void InstructionBinaryOp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCall::InstructionCall(Type type, uint64_t callee)
        : Value(type), callee(callee) {}

    uint64_t InstructionCall::getCallee() { return callee; }

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorReturn::TerminatorReturn(Value *value) : value(value) {}

    Value *TerminatorReturn::getValue() { return value; }

    void TerminatorReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace middleend::mir
