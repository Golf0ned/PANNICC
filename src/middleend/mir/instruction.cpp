#include "middleend/mir/instruction.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    InstructionBinaryOp::InstructionBinaryOp(Type type, BinaryOp op,
                                             Value *left, Value *right)
        : Value(type), op(op), left(left), right(right) {
        left->getUses().push_back(this);
        right->getUses().push_back(this);
    }

    BinaryOp InstructionBinaryOp::getOp() { return op; }

    Value *InstructionBinaryOp::getLeft() { return left; }

    Value *InstructionBinaryOp::getRight() { return right; }

    void InstructionBinaryOp::setLeft(Value *newVal) { left = newVal; }

    void InstructionBinaryOp::setRight(Value *newVal) { right = newVal; }

    void InstructionBinaryOp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCall::InstructionCall(Type type, Function *callee)
        : Value(type), callee(callee) {}

    Function *InstructionCall::getCallee() { return callee; }

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionAlloca::InstructionAlloca(Type allocType)
        : Value(Type::PTR), allocType(allocType) {}

    Type InstructionAlloca::getAllocType() { return allocType; }

    void InstructionAlloca::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionLoad::InstructionLoad(Type type, InstructionAlloca *ptr)
        : Value(type), ptr(ptr) {
        ptr->getUses().push_back(this);
    }

    InstructionAlloca *InstructionLoad::getPtr() { return ptr; }

    void InstructionLoad::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionStore::InstructionStore(Value *value, InstructionAlloca *ptr)
        : value(value), ptr(ptr) {
        value->getUses().push_back(this);
        ptr->getUses().push_back(this);
    }

    Value *InstructionStore::getValue() { return value; }

    InstructionAlloca *InstructionStore::getPtr() { return ptr; }

    void InstructionStore::setValue(Value *newVal) { value = newVal; }

    void InstructionStore::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorReturn::TerminatorReturn(Value *value) : value(value) {
        value->getUses().push_back(this);
    }

    Value *TerminatorReturn::getValue() { return value; }

    void TerminatorReturn::setValue(Value *newVal) { value = newVal; }

    void TerminatorReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace middleend::mir
