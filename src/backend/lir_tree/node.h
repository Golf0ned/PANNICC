#pragma once

#include <list>
#include <unordered_set>

#include "backend/lir/instruction.h"
#include "middleend/mir/operator.h"

namespace backend::lir_tree {
    class NodeVisitor;

    enum class NodeType {
        NONE,
        REGISTER,
        IMMEDIATE,
        ADDRESS,
        OP,
        LOAD,
        STORE,
        ASM,
    };

    class Node {
    public:
        virtual void accept(NodeVisitor *v) = 0;
        virtual ~Node() = default;
    };

    class RegisterNode : public Node {
    public:
        RegisterNode(std::string name, std::unique_ptr<Node> source);
        std::string getName();
        std::unique_ptr<Node> &getSource();
        bool sameReg(RegisterNode *other);
        void consume(RegisterNode *other);
        void accept(NodeVisitor *v);

    private:
        std::string name;
        std::unique_ptr<Node> source;
    };

    class ImmediateNode : public Node {
    public:
        ImmediateNode(uint64_t value);
        uint64_t getValue();
        void accept(NodeVisitor *v);

    private:
        uint64_t value;
    };

    class AddressNode : public Node {
    public:
        AddressNode(std::unique_ptr<RegisterNode> base,
                    std::unique_ptr<RegisterNode> index, uint64_t scale,
                    uint64_t displacement);
        std::unique_ptr<RegisterNode> &getBase();
        std::unique_ptr<RegisterNode> &getIndex();
        uint64_t getScale();
        uint64_t getDisplacement();
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<RegisterNode> base;
        std::unique_ptr<RegisterNode> index;
        uint64_t scale;
        uint64_t displacement;
    };

    class OpNode : public Node {
    public:
        OpNode(middleend::mir::BinaryOp op, std::unique_ptr<Node> left,
               std::unique_ptr<Node> right);
        middleend::mir::BinaryOp getOp();
        std::unique_ptr<Node> &getLeft();
        std::unique_ptr<Node> &getRight();
        void accept(NodeVisitor *v);

    private:
        middleend::mir::BinaryOp op;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    class LoadNode : public Node {
    public:
        LoadNode(std::unique_ptr<Node> ptr);
        std::unique_ptr<Node> &getPtr();
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> ptr;
    };

    class StoreNode : public Node {
    public:
        StoreNode(std::unique_ptr<Node> source, std::unique_ptr<Node> ptr);
        std::unique_ptr<Node> &getSource();
        std::unique_ptr<Node> &getPtr();
        void accept(NodeVisitor *v);

    private:
        std::unique_ptr<Node> source;
        std::unique_ptr<Node> ptr;
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
        virtual void visit(AddressNode *n) = 0;
        virtual void visit(ImmediateNode *n) = 0;
        virtual void visit(OpNode *n) = 0;
        virtual void visit(LoadNode *n) = 0;
        virtual void visit(StoreNode *n) = 0;
        virtual void visit(AsmNode *n) = 0;
    };

    class ToStringVisitor : public NodeVisitor {
    public:
        ToStringVisitor(lir::OperandManager *om);

        std::string getResult();

        void visit(Node *n) override;
        void visit(RegisterNode *n) override;
        void visit(ImmediateNode *n) override;
        void visit(AddressNode *n) override;
        void visit(OpNode *n) override;
        void visit(LoadNode *n) override;
        void visit(StoreNode *n) override;
        void visit(AsmNode *n) override;

    private:
        lir::OperandManager *om;
        std::string result;
    };

    class Forest {
    public:
        void insertAsm(std::unique_ptr<Node> tree);
        void insertTree(std::unique_ptr<Node> tree, std::list<Node *> leaves,
                        bool has_memory_instruction);
        std::unique_ptr<Node> pop();
        std::list<Node *> &getLeaves(Node *tree);
        bool hasMemInst(Node *tree);
        void setMemInst(Node *tree);
        bool empty();
        std::string toString(lir::OperandManager *om);

    private:
        std::list<std::unique_ptr<Node>> trees;
        std::unordered_map<Node *, std::list<Node *>> tree_leaves;
        std::unordered_set<Node *> trees_with_memory_instruction;
    };
} // namespace backend::lir_tree
