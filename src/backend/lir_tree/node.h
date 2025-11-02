#pragma once

#include <list>

#include "backend/lir/instruction.h"
#include "middleend/mir/operator.h"

namespace backend::lir_tree {
    class NodeVisitor;

    class Node {
    public:
        virtual void accept(NodeVisitor *v) = 0;
        virtual ~Node() = default;
    };

    class RegisterNode : public Node {
    public:
        RegisterNode(std::string name);
        void setSource(std::unique_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::string name;
        std::unique_ptr<Node> source;
    };

    class ImmediateNode : public Node {
    public:
        ImmediateNode(uint64_t value);
        void accept(NodeVisitor *v);

    private:
        uint64_t value;
    };

    class OpNode : public Node {
    public:
        OpNode(middleend::mir::BinaryOp op);
        void setLeft(std::unique_ptr<Node> new_node);
        void setRight(std::unique_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        middleend::mir::BinaryOp op;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class AllocaNode : public Node {
    public:
        void setSize(std::unique_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> size;
    };

    class LoadNode : public Node {
    public:
        void setPtr(std::unique_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> ptr;
    };

    class StoreNode : public Node {
    public:
        void setSource(std::unique_ptr<Node> new_node);
        void setPtr(std::unique_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> source;
        std::unique_ptr<Node> ptr;
    };

    class PhiNode : public Node {
    public:
        PhiNode(std::unique_ptr<Node> to,
                std::list<std::unique_ptr<Node>> from);
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> to;
        std::list<std::unique_ptr<Node>> from;
    };

    class AsmNode : public Node {
    public:
        AsmNode(std::list<std::unique_ptr<lir::Instruction>> assembly);
        std::list<std::unique_ptr<lir::Instruction>> &getAssembly();
        void accept(NodeVisitor *v);

    private:
        std::list<std::unique_ptr<lir::Instruction>> assembly;
    };

    class NodeVisitor {
    public:
        virtual void visit(Node *n) = 0;
        virtual void visit(RegisterNode *n) = 0;
        virtual void visit(ImmediateNode *n) = 0;
        virtual void visit(OpNode *n) = 0;
        virtual void visit(AllocaNode *n) = 0;
        virtual void visit(LoadNode *n) = 0;
        virtual void visit(StoreNode *n) = 0;
        virtual void visit(PhiNode *n) = 0;
        virtual void visit(AsmNode *n) = 0;
    };
} // namespace backend::lir_tree
