#pragma once

#include <list>
#include <unordered_set>

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
        void setSource(std::shared_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::string name;
        std::shared_ptr<Node> source;
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
        void setLeft(std::shared_ptr<Node> new_node);
        void setRight(std::shared_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        middleend::mir::BinaryOp op;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };

    class AllocaNode : public Node {
    public:
        void setSize(std::shared_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::shared_ptr<Node> size;
    };

    class LoadNode : public Node {
    public:
        void setPtr(std::shared_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::shared_ptr<Node> ptr;
    };

    class StoreNode : public Node {
    public:
        void setSource(std::shared_ptr<Node> new_node);
        void setPtr(std::shared_ptr<Node> new_node);
        void accept(NodeVisitor *v);

    private:
        std::shared_ptr<Node> source;
        std::shared_ptr<Node> ptr;
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
        virtual void visit(AsmNode *n) = 0;
    };

    class Forest {
    public:
        void insertAsm(std::shared_ptr<Node> tree);
        void insertTree(std::shared_ptr<Node> tree,
                        std::vector<std::shared_ptr<Node>> leaves,
                        bool has_memory_instruction);
        std::shared_ptr<Node> pop();
        std::vector<std::shared_ptr<Node>> &getLeaves(Node *tree);
        bool hasMemoryInstruction(Node *tree);
        bool empty();

    private:
        std::list<std::shared_ptr<Node>> trees;
        std::unordered_map<Node *, std::vector<std::shared_ptr<Node>>>
            tree_leaves;
        std::unordered_set<Node *> trees_with_memory_instruction;
    };
} // namespace backend::lir_tree
