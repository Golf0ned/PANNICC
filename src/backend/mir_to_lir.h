#pragma once

#include "backend/lir/lir.h"
#include "backend/lir/operand.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir);

    class Node {};

    class RegisterNode : public Node {
    public:
        RegisterNode(std::string name);
        void setSource(std::unique_ptr<Node> new_node);

    private:
        std::string name;
        std::unique_ptr<Node> source;
    };

    class ImmediateNode : public Node {
    public:
        ImmediateNode(uint64_t value);

    private:
        uint64_t value;
    };

    class OpNode : public Node {
    public:
        OpNode(middleend::mir::BinaryOp op);
        void setLeft(std::unique_ptr<Node> new_node);
        void setRight(std::unique_ptr<Node> new_node);

    private:
        middleend::mir::BinaryOp op;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class AllocaNode : public Node {
    public:
        void setSize(std::unique_ptr<Node> new_node);

    private:
        std::unique_ptr<Node> size;
    };

    class LoadNode : public Node {
    public:
        void setPtr(std::unique_ptr<Node> new_node);

    private:
        std::unique_ptr<Node> ptr;
    };

    class StoreNode : public Node {
    public:
        void setSource(std::unique_ptr<Node> new_node);
        void setPtr(std::unique_ptr<Node> new_node);

    private:
        std::unique_ptr<Node> source;
        std::unique_ptr<Node> ptr;
    };

    class PhiNode : public Node {
    public:
        PhiNode(std::unique_ptr<Node> to,
                std::list<std::unique_ptr<Node>> from);

    private:
        std::unique_ptr<Node> to;
        std::list<std::unique_ptr<Node>> from;
    };

    class AsmNode : public Node {
    public:
        AsmNode(std::list<std::unique_ptr<lir::Instruction>> assembly);

    private:
        std::list<std::unique_ptr<lir::Instruction>> assembly;
    };

    class TreeGenVisitor : public middleend::mir::InstructionVisitor {
    public:
        TreeGenVisitor(middleend::mir::Program &p, lir::OperandManager &om);

        std::list<std::list<std::unique_ptr<Node>>> getResult();

        void startFunction(middleend::mir::Function *f);
        void endFunction();
        void startBasicBlock(middleend::mir::BasicBlock *bb,
                             middleend::mir::BasicBlock *next_bb);
        std::unique_ptr<Node> resolveValue(middleend::mir::Value *v);

        virtual void visit(middleend::mir::InstructionBinaryOp *i);
        virtual void visit(middleend::mir::InstructionCall *i);
        virtual void visit(middleend::mir::InstructionAlloca *i);
        virtual void visit(middleend::mir::InstructionLoad *i);
        virtual void visit(middleend::mir::InstructionStore *i);
        virtual void visit(middleend::mir::InstructionPhi *i);

        virtual void visit(middleend::mir::TerminatorReturn *t);
        virtual void visit(middleend::mir::TerminatorBranch *t);
        virtual void visit(middleend::mir::TerminatorCondBranch *t);

    private:
        middleend::NumberIR nir;
        lir::OperandManager &om;
        std::string function_name;
        middleend::mir::BasicBlock *next_block;
        std::list<std::unique_ptr<Node>> function_trees;
        std::list<std::list<std::unique_ptr<Node>>> program_trees;
    };
} // namespace backend
