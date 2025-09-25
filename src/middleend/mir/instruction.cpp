#include <stdexcept>

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

    void InstructionBinaryOp::setLeft(Value *new_val) { left = new_val; }

    void InstructionBinaryOp::setRight(Value *new_val) { right = new_val; }

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

    void InstructionStore::setValue(Value *new_val) { value = new_val; }

    void InstructionStore::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionPhi::InstructionPhi(Type type) : Value(type) {}

    std::unordered_map<BasicBlock *, Value *> &
    InstructionPhi::getPredecessors() {
        return predecessors;
    }

    void InstructionPhi::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorReturn::TerminatorReturn(Value *value) : value(value) {
        value->getUses().push_back(this);
    }

    Value *TerminatorReturn::getValue() { return value; }

    void TerminatorReturn::setValue(Value *new_val) { value = new_val; }

    void TerminatorReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorBranch::TerminatorBranch(BasicBlock *successor)
        : successor(successor) {}

    BasicBlock *TerminatorBranch::getSuccessor() { return successor; }

    void TerminatorBranch::setSuccessor(BasicBlock *successor) {
        this->successor = successor;
    }

    void TerminatorBranch::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    TerminatorCondBranch::TerminatorCondBranch(Value *cond,
                                               BasicBlock *t_successor,
                                               BasicBlock *f_successor)
        : t_successor(t_successor), f_successor(f_successor) {
        // if (cond->getType() != Type::I1)
        //     throw std::invalid_argument("TerminatorCondBranch cond must be
        //     i1");
        this->cond = cond;
    }

    Value *TerminatorCondBranch::getCond() { return cond; }

    BasicBlock *TerminatorCondBranch::getTSuccessor() { return t_successor; };

    BasicBlock *TerminatorCondBranch::getFSuccessor() { return f_successor; };

    void TerminatorCondBranch::setCond(Value *new_val) { this->cond = new_val; }

    void TerminatorCondBranch::setTSuccessor(BasicBlock *t_successor) {
        this->t_successor = t_successor;
    }

    void TerminatorCondBranch::setFSuccessor(BasicBlock *f_successor) {
        this->f_successor = f_successor;
    }

    void TerminatorCondBranch::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace middleend::mir
