#pragma once

#include <list>

#include "backend/lir/instruction.h"
#include "middleend/mir/operator.h"

namespace backend::lir_tree {
    class Node {
    public:
    };

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
} // namespace backend::lir_tree
