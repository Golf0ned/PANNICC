#include "middleend/mir/instruction.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    void Instruction::addUse(Value *def) { def->getUses()[this]++; }

    void Instruction::delUse(Value *def) {
        def->getUses()[this]--;
        if (def->getUses()[this] == 0)
            def->getUses().erase(this);
    }

    InstructionBinaryOp::InstructionBinaryOp(Type type, BinaryOp op,
                                             Value *left, Value *right)
        : Value(type), op(op), left(left), right(right) {
        addUse(left);
        addUse(right);
    }

    BinaryOp InstructionBinaryOp::getOp() { return op; }

    Value *InstructionBinaryOp::getLeft() { return left; }

    Value *InstructionBinaryOp::getRight() { return right; }

    void InstructionBinaryOp::setLeft(Value *new_val) {
        delUse(left);
        addUse(new_val);
        left = new_val;
    }

    void InstructionBinaryOp::setRight(Value *new_val) {
        delUse(right);
        addUse(new_val);
        right = new_val;
    }

    void InstructionBinaryOp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCall::InstructionCall(Type type, Function *callee,
                                     std::vector<Value *> arguments)
        : Value(type), callee(callee), arguments(arguments) {
        for (auto val : arguments)
            addUse(val);
    }

    Function *InstructionCall::getCallee() { return callee; }

    void InstructionCall::setCallee(Function *new_val) { callee = new_val; }

    std::vector<Value *> &InstructionCall::getArguments() { return arguments; }

    void InstructionCall::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionAlloca::InstructionAlloca(Type allocType)
        : Value(Type::PTR), allocType(allocType) {}

    Type InstructionAlloca::getAllocType() { return allocType; }

    void InstructionAlloca::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionLoad::InstructionLoad(Type type, InstructionAlloca *ptr)
        : Value(type), ptr(ptr) {
        addUse(ptr);
    }

    InstructionAlloca *InstructionLoad::getPtr() { return ptr; }

    void InstructionLoad::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionStore::InstructionStore(Value *value, InstructionAlloca *ptr)
        : value(value), ptr(ptr) {
        addUse(value);
        addUse(ptr);
    }

    Value *InstructionStore::getValue() { return value; }

    InstructionAlloca *InstructionStore::getPtr() { return ptr; }

    void InstructionStore::setValue(Value *new_val) {
        delUse(value);
        addUse(new_val);
        value = new_val;
    }

    void InstructionStore::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPhi::InstructionPhi(Type type) : Value(type) {}

    std::unordered_map<BasicBlock *, Value *> &
    InstructionPhi::getPredecessors() {
        return predecessors;
    }

    void InstructionPhi::setPredecessor(BasicBlock *bb, Value *new_val) {
        if (predecessors.contains(bb))
            delUse(predecessors.at(bb));
        addUse(new_val);
        predecessors[bb] = new_val;
    }

    void InstructionPhi::accept(InstructionVisitor *v) { v->visit(this); }

    std::unordered_map<InstructionPhi *, Value *> &
    InstructionParallelCopy::getCopies() {
        return copies;
    }

    void InstructionParallelCopy::setCopy(InstructionPhi *phi_val,
                                          Value *copied_val) {
        if (copies.contains(phi_val))
            delUse(copies.at(phi_val));
        else
            addUse(phi_val);
        addUse(copied_val);
        copies[phi_val] = copied_val;
    }

    void InstructionParallelCopy::accept(InstructionVisitor *v) {
        v->visit(this);
    }

    TerminatorReturn::TerminatorReturn(Value *value) : value(value) {
        addUse(value);
    }

    Value *TerminatorReturn::getValue() { return value; }

    void TerminatorReturn::setValue(Value *new_val) {
        delUse(value);
        addUse(new_val);
        value = new_val;
    }

    void TerminatorReturn::accept(InstructionVisitor *v) { v->visit(this); }

    TerminatorBranch::TerminatorBranch(BasicBlock *successor)
        : successor(successor) {}

    BasicBlock *TerminatorBranch::getSuccessor() { return successor; }

    void TerminatorBranch::setSuccessor(BasicBlock *successor) {
        this->successor = successor;
    }

    void TerminatorBranch::accept(InstructionVisitor *v) { v->visit(this); }

    TerminatorCondBranch::TerminatorCondBranch(Value *cond,
                                               BasicBlock *t_successor,
                                               BasicBlock *f_successor)
        : t_successor(t_successor), f_successor(f_successor) {
        this->cond = cond;
        addUse(cond);
    }

    Value *TerminatorCondBranch::getCond() { return cond; }

    BasicBlock *TerminatorCondBranch::getTSuccessor() { return t_successor; };

    BasicBlock *TerminatorCondBranch::getFSuccessor() { return f_successor; };

    void TerminatorCondBranch::setCond(Value *new_val) {
        delUse(cond);
        addUse(new_val);
        this->cond = new_val;
    }

    void TerminatorCondBranch::setTSuccessor(BasicBlock *t_successor) {
        this->t_successor = t_successor;
    }

    void TerminatorCondBranch::setFSuccessor(BasicBlock *f_successor) {
        this->f_successor = f_successor;
    }

    void TerminatorCondBranch::accept(InstructionVisitor *v) { v->visit(this); }
} // namespace middleend::mir
