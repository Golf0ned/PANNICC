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

    InstructionAlloca::InstructionAlloca(Type allocType)
        : Value(Type::PTR), allocType(allocType) {}

    Type InstructionAlloca::getAllocType() { return allocType; }

    void InstructionAlloca::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionLoad::InstructionLoad(Type type, Value *ptr)
        : Value(type), ptr(ptr) {}

    Value *InstructionLoad::getPtr() { return ptr; }

    void InstructionLoad::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionStore::InstructionStore(Value *value, Value *ptr)
        : value(value), ptr(ptr) {}

    Value *InstructionStore::getValue() { return value; }

    Value *InstructionStore::getPtr() { return ptr; }

    void InstructionStore::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorReturn::TerminatorReturn(Value *value) : value(value) {}

    Value *TerminatorReturn::getValue() { return value; }

    void TerminatorReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace middleend::mir
