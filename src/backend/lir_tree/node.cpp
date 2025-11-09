#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    RegisterNode::RegisterNode(std::string name) : name(name) {}

    void RegisterNode::setSource(std::shared_ptr<Node> new_node) {
        source = std::move(new_node);
    }

    void RegisterNode::accept(NodeVisitor *v) { v->visit(this); }

    ImmediateNode::ImmediateNode(uint64_t value) : value(value) {}

    void ImmediateNode::accept(NodeVisitor *v) { v->visit(this); }

    OpNode::OpNode(middleend::mir::BinaryOp op) : op(op) {}

    void OpNode::setLeft(std::shared_ptr<Node> new_node) {
        left = std::move(new_node);
    }

    void OpNode::setRight(std::shared_ptr<Node> new_node) {
        right = std::move(new_node);
    }

    void OpNode::accept(NodeVisitor *v) { v->visit(this); }

    void AllocaNode::setSize(std::shared_ptr<Node> new_node) {
        size = std::move(new_node);
    }

    void AllocaNode::accept(NodeVisitor *v) { v->visit(this); }

    void LoadNode::setPtr(std::shared_ptr<Node> new_node) {
        ptr = std::move(new_node);
    }

    void LoadNode::accept(NodeVisitor *v) { v->visit(this); }

    void StoreNode::setSource(std::shared_ptr<Node> new_node) {
        source = std::move(new_node);
    }
    void StoreNode::setPtr(std::shared_ptr<Node> new_node) {
        ptr = std::move(new_node);
    }
    void StoreNode::accept(NodeVisitor *v) { v->visit(this); }

    AsmNode::AsmNode(std::list<std::unique_ptr<lir::Instruction>> assembly)
        : assembly(std::move(assembly)) {}

    std::list<std::unique_ptr<lir::Instruction>> &AsmNode::getAssembly() {
        return assembly;
    }

    void AsmNode::accept(NodeVisitor *v) { v->visit(this); }

    void Forest::insertAsm(std::shared_ptr<Node> tree) {
        trees.push_back(std::move(tree));
    }

    void Forest::insertTree(std::shared_ptr<Node> tree,
                            std::vector<std::shared_ptr<Node>> leaves,
                            bool has_memory_instruction) {
        tree_leaves[tree.get()] = std::move(leaves);
        if (has_memory_instruction) {
            trees_with_memory_instruction.insert(tree.get());
        }
        trees.push_back(std::move(tree));
    }

    std::shared_ptr<Node> Forest::pop() {
        auto res = std::move(trees.back());
        trees.pop_back();
        return std::move(res);
    }

    std::vector<std::shared_ptr<Node>> &Forest::getLeaves(Node *tree) {
        return tree_leaves.at(tree);
    }

    bool Forest::hasMemoryInstruction(Node *tree) {
        return trees_with_memory_instruction.contains(tree);
    }

    bool Forest::empty() { return trees.empty(); }
} // namespace backend::lir_tree
