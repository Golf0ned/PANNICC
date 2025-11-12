#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    RegisterNode::RegisterNode(std::string name) : name(name) {}

    std::string RegisterNode::getName() { return name; }

    Node *RegisterNode::getSource() {
        if (!source)
            return nullptr;
        return source.get();
    }

    void RegisterNode::setSource(std::shared_ptr<Node> new_node) {
        source = std::move(new_node);
    }

    bool RegisterNode::sameReg(RegisterNode *other) {
        return name == other->name;
    }

    void RegisterNode::consume(RegisterNode *other) {
        source = std::move(other->source);
    }

    void RegisterNode::accept(NodeVisitor *v) { v->visit(this); }

    ImmediateNode::ImmediateNode(uint64_t value) : value(value) {}

    uint64_t ImmediateNode::getValue() { return value; }

    void ImmediateNode::accept(NodeVisitor *v) { v->visit(this); }

    OpNode::OpNode(middleend::mir::BinaryOp op) : op(op) {}

    middleend::mir::BinaryOp OpNode::getOp() { return op; }

    Node *OpNode::getLeft() {
        if (!left)
            return nullptr;
        return left.get();
    }

    Node *OpNode::getRight() {
        if (!right)
            return nullptr;
        return right.get();
    }

    void OpNode::setLeft(std::shared_ptr<Node> new_node) {
        left = std::move(new_node);
    }

    void OpNode::setRight(std::shared_ptr<Node> new_node) {
        right = std::move(new_node);
    }

    void OpNode::accept(NodeVisitor *v) { v->visit(this); }

    Node *AllocaNode::getSize() {
        if (!size)
            return nullptr;
        return size.get();
    }

    void AllocaNode::setSize(std::shared_ptr<Node> new_node) {
        size = std::move(new_node);
    }

    void AllocaNode::accept(NodeVisitor *v) { v->visit(this); }

    Node *LoadNode::getPtr() {
        if (!ptr)
            return nullptr;
        return ptr.get();
    }

    void LoadNode::setPtr(std::shared_ptr<Node> new_node) {
        ptr = std::move(new_node);
    }

    void LoadNode::accept(NodeVisitor *v) { v->visit(this); }

    Node *StoreNode::getSource() {
        if (!source)
            return nullptr;
        return source.get();
    }

    Node *StoreNode::getPtr() {
        if (!ptr)
            return nullptr;
        return ptr.get();
    }

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

    PrintNodeVisitor::PrintNodeVisitor(lir::OperandManager &om) : om(om) {}

    std::string PrintNodeVisitor::getResult() { return result; }

    void PrintNodeVisitor::visit(Node *n) {}

    void PrintNodeVisitor::visit(RegisterNode *n) {
        if (!n->getSource()) {
            result = "%" + n->getName();
            return;
        }

        n->getSource()->accept(this);
        result = "%" + n->getName() + "(" + result + ")";
    }

    void PrintNodeVisitor::visit(ImmediateNode *n) {
        result = "imm(" + std::to_string(n->getValue()) + ")";
    }

    void PrintNodeVisitor::visit(OpNode *n) {
        std::string op = middleend::mir::toString(n->getOp());

        n->getLeft()->accept(this);
        std::string left = result;

        n->getRight()->accept(this);
        std::string right = result;

        result = op + "(" + left + ", " + right + ")";
    }

    void PrintNodeVisitor::visit(AllocaNode *n) {
        n->getSize()->accept(this);
        std::string size = result;

        result = "alloca(" + size + ")";
    }

    void PrintNodeVisitor::visit(LoadNode *n) {
        n->getPtr()->accept(this);
        std::string ptr = result;

        result = "load(" + ptr + ")";
    }

    void PrintNodeVisitor::visit(StoreNode *n) {
        n->getSource()->accept(this);
        std::string source = result;

        n->getPtr()->accept(this);
        std::string ptr = result;

        result = "store(" + source + ", " + ptr + ")";
    }

    void PrintNodeVisitor::visit(AsmNode *n) { result = "AsmNode"; }

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

    void Forest::propagateMemoryInstruction(Node *tree) {
        trees_with_memory_instruction.insert(tree);
    }

    bool Forest::empty() { return trees.empty(); }

    std::string Forest::toString(lir::OperandManager &om) {
        PrintNodeVisitor pnv(om);
        std::string res;
        for (auto tree : trees) {
            tree->accept(&pnv);
            res += pnv.getResult() + '\n';
        }

        return res;
    }

} // namespace backend::lir_tree
