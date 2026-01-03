#pragma once

#include <vector>

#include "middleend/mir/operator.h"
#include "middleend/mir/value.h"

namespace middleend::mir {

    class InstructionVisitor;
    class Function;
    class BasicBlock;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *v) = 0;
        virtual ~Instruction() = default;
        void addUse(Value *def);
        void delUse(Value *def);
    };

    class InstructionBinaryOp : public Instruction, public Value {
    public:
        InstructionBinaryOp(Type type, BinaryOp op, Value *left, Value *right);
        BinaryOp getOp();
        Value *getLeft();
        Value *getRight();
        void setLeft(Value *new_val);
        void setRight(Value *new_val);
        void accept(InstructionVisitor *v) override;

    private:
        BinaryOp op;
        Value *left;
        Value *right;
    };

    class InstructionCall : public Instruction, public Value {
    public:
        InstructionCall(Type type, Function *callee,
                        std::vector<Value *> arguments);
        Function *getCallee();
        std::vector<Value *> &getArguments();
        void accept(InstructionVisitor *v) override;

    private:
        Function *callee;
        std::vector<Value *> arguments;
    };

    class InstructionAlloca : public Instruction, public Value {
    public:
        InstructionAlloca(Type allocType);
        Type getAllocType();
        void accept(InstructionVisitor *v) override;

    private:
        Type allocType;
    };

    class InstructionLoad : public Instruction, public Value {
    public:
        InstructionLoad(Type type, InstructionAlloca *ptr);
        InstructionAlloca *getPtr();
        void accept(InstructionVisitor *v) override;

    private:
        InstructionAlloca *ptr;
    };

    class InstructionStore : public Instruction {
    public:
        InstructionStore(Value *value, InstructionAlloca *ptr);
        Value *getValue();
        InstructionAlloca *getPtr();
        void setValue(Value *new_val);
        void accept(InstructionVisitor *v) override;

    private:
        Value *value;
        InstructionAlloca *ptr;
    };

    class InstructionPhi : public Instruction, public Value {
    public:
        InstructionPhi(Type type);
        std::unordered_map<BasicBlock *, Value *> &getPredecessors();
        void setPredecessor(BasicBlock *bb, Value *new_val);
        void accept(InstructionVisitor *v) override;

    private:
        std::unordered_map<BasicBlock *, Value *> predecessors;
    };

    class InstructionParallelCopy : public Instruction {
    public:
        std::unordered_map<InstructionPhi *, Value *> &getCopies();
        void setCopy(InstructionPhi *phi_val, Value *copied_val);
        void accept(InstructionVisitor *v) override;

    private:
        std::unordered_map<InstructionPhi *, Value *> copies;
    };

    class Terminator : public Instruction {
    public:
        virtual void accept(InstructionVisitor *v) = 0;
        virtual ~Terminator() = default;
    };

    class TerminatorReturn : public Terminator {
    public:
        TerminatorReturn(Value *value);
        Value *getValue();
        void setValue(Value *new_val);
        void accept(InstructionVisitor *v) override;

    private:
        Value *value;
    };

    class TerminatorBranch : public Terminator {
    public:
        TerminatorBranch(BasicBlock *successor);
        BasicBlock *getSuccessor();
        void setSuccessor(BasicBlock *successor);
        void accept(InstructionVisitor *v) override;

    private:
        BasicBlock *successor;
    };

    class TerminatorCondBranch : public Terminator {
    public:
        TerminatorCondBranch(Value *cond, BasicBlock *t_successor,
                             BasicBlock *f_successor);
        Value *getCond();
        BasicBlock *getTSuccessor();
        BasicBlock *getFSuccessor();
        void setCond(Value *new_val);
        void setTSuccessor(BasicBlock *t_successor);
        void setFSuccessor(BasicBlock *f_successor);
        void accept(InstructionVisitor *v) override;

    private:
        Value *cond;
        BasicBlock *t_successor;
        BasicBlock *f_successor;
    };

    class InstructionVisitor {
    public:
        virtual void visit(InstructionBinaryOp *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionAlloca *i) = 0;
        virtual void visit(InstructionLoad *i) = 0;
        virtual void visit(InstructionStore *i) = 0;
        virtual void visit(InstructionPhi *i) = 0;
        virtual void visit(InstructionParallelCopy *i) = 0;

        virtual void visit(TerminatorReturn *t) = 0;
        virtual void visit(TerminatorBranch *t) = 0;
        virtual void visit(TerminatorCondBranch *t) = 0;
    };
} // namespace middleend::mir
