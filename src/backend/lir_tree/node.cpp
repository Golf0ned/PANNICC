#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    RegisterNode::RegisterNode(std::string name, std::unique_ptr<Node> source)
        : name(name), source(std::move(source)) {}

    std::string RegisterNode::getName() { return name; }

    std::unique_ptr<Node> &RegisterNode::getSource() { return source; }

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

    AddressNode::AddressNode(std::unique_ptr<RegisterNode> base,
                             std::unique_ptr<RegisterNode> index,
                             uint64_t scale, uint64_t displacement)
        : base(std::move(base)), index(std::move(index)), scale(scale),
          displacement(displacement) {}

    std::unique_ptr<RegisterNode> &AddressNode::getBase() { return base; }

    std::unique_ptr<RegisterNode> &AddressNode::getIndex() { return index; }

    uint64_t AddressNode::getScale() { return scale; }

    uint64_t AddressNode::getDisplacement() { return displacement; }

    void AddressNode::accept(NodeVisitor *v) { v->visit(this); }

    OpNode::OpNode(middleend::mir::BinaryOp op, std::unique_ptr<Node> left,
                   std::unique_ptr<Node> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    middleend::mir::BinaryOp OpNode::getOp() { return op; }

    std::unique_ptr<Node> &OpNode::getLeft() { return left; }

    std::unique_ptr<Node> &OpNode::getRight() { return right; }

    void OpNode::accept(NodeVisitor *v) { v->visit(this); }

    LoadNode::LoadNode(std::unique_ptr<Node> ptr) : ptr(std::move(ptr)) {}

    std::unique_ptr<Node> &LoadNode::getPtr() { return ptr; }

    void LoadNode::accept(NodeVisitor *v) { v->visit(this); }

    StoreNode::StoreNode(std::unique_ptr<Node> source,
                         std::unique_ptr<Node> ptr)
        : source(std::move(source)), ptr(std::move(ptr)) {}

    std::unique_ptr<Node> &StoreNode::getSource() { return source; }

    std::unique_ptr<Node> &StoreNode::getPtr() { return ptr; }

    void StoreNode::accept(NodeVisitor *v) { v->visit(this); }

    AsmNode::AsmNode(std::list<std::unique_ptr<lir::Instruction>> assembly)
        : assembly(std::move(assembly)) {}

    std::list<std::unique_ptr<lir::Instruction>> &AsmNode::getAssembly() {
        return assembly;
    }

    void AsmNode::accept(NodeVisitor *v) { v->visit(this); }

    ToStringVisitor::ToStringVisitor(lir::OperandManager *om) : om(om) {}

    std::string ToStringVisitor::getResult() { return result; }

    void ToStringVisitor::visit(Node *n) {}

    void ToStringVisitor::visit(RegisterNode *n) {
        if (!n->getSource()) {
            result = "%" + n->getName();
            return;
        }

        n->getSource()->accept(this);
        result = "%" + n->getName() + "(" + result + ")";
    }

    void ToStringVisitor::visit(ImmediateNode *n) {
        result = "imm(" + std::to_string(n->getValue()) + ")";
    }

    void ToStringVisitor::visit(AddressNode *n) {
        auto &base_node = n->getBase();
        std::string base = "null";
        if (base_node) {
            base_node->accept(this);
            base = result;
        }

        auto &index_node = n->getIndex();
        std::string index = "null";
        if (index_node) {
            index_node->accept(this);
            index = result;
        }

        std::string scale = std::to_string(n->getScale());
        std::string displacement = std::to_string(n->getDisplacement());

        result = "addr(" + base + ", " + index + ", " + scale + ", " +
                 displacement + ")";
    }

    void ToStringVisitor::visit(OpNode *n) {
        std::string op = middleend::mir::toString(n->getOp());

        n->getLeft()->accept(this);
        std::string left = result;

        n->getRight()->accept(this);
        std::string right = result;

        result = op + "(" + left + ", " + right + ")";
    }

    void ToStringVisitor::visit(LoadNode *n) {
        n->getPtr()->accept(this);
        std::string ptr = result;

        result = "load(" + ptr + ")";
    }

    void ToStringVisitor::visit(StoreNode *n) {
        n->getSource()->accept(this);
        std::string source = result;

        n->getPtr()->accept(this);
        std::string ptr = result;

        result = "store(" + source + ", " + ptr + ")";
    }

    void ToStringVisitor::visit(AsmNode *n) { result = "AsmNode"; }

    void Forest::insertAsm(std::unique_ptr<Node> tree) {
        trees.push_back(std::move(tree));
    }

    void Forest::insertTree(std::unique_ptr<Node> tree,
                            std::list<Node *> leaves,
                            bool has_memory_instruction) {
        tree_leaves[tree.get()] = std::move(leaves);
        if (has_memory_instruction)
            trees_with_memory_instruction.insert(tree.get());
        trees.push_back(std::move(tree));
    }

    std::unique_ptr<Node> Forest::pop() {
        auto res = std::move(trees.back());
        trees.pop_back();
        return std::move(res);
    }

    std::list<Node *> &Forest::getLeaves(Node *tree) {
        return tree_leaves[tree];
    }

    bool Forest::hasMemInst(Node *tree) {
        return trees_with_memory_instruction.contains(tree);
    }

    void Forest::setMemInst(Node *tree) {
        trees_with_memory_instruction.insert(tree);
    }

    bool Forest::empty() { return trees.empty(); }

    std::string Forest::toString(lir::OperandManager *om) {
        ToStringVisitor tsv(om);
        std::string res;
        for (auto &tree : trees) {
            tree->accept(&tsv);
            res += tsv.getResult() + '\n';
        }

        return res;
    }
} // namespace backend::lir_tree
