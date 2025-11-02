#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    RegisterNode::RegisterNode(std::string name) : name(name) {}

    void RegisterNode::setSource(std::unique_ptr<Node> new_node) {
        source = std::move(new_node);
    }

    void RegisterNode::accept(NodeVisitor *v) { v->visit(this); }

    ImmediateNode::ImmediateNode(uint64_t value) : value(value) {}

    void ImmediateNode::accept(NodeVisitor *v) { v->visit(this); }

    OpNode::OpNode(middleend::mir::BinaryOp op) : op(op) {}

    void OpNode::setLeft(std::unique_ptr<Node> new_node) {
        left = std::move(new_node);
    }

    void OpNode::setRight(std::unique_ptr<Node> new_node) {
        right = std::move(new_node);
    }

    void OpNode::accept(NodeVisitor *v) { v->visit(this); }

    void AllocaNode::setSize(std::unique_ptr<Node> new_node) {
        size = std::move(new_node);
    }

    void AllocaNode::accept(NodeVisitor *v) { v->visit(this); }

    void LoadNode::setPtr(std::unique_ptr<Node> new_node) {
        ptr = std::move(new_node);
    }

    void LoadNode::accept(NodeVisitor *v) { v->visit(this); }

    void StoreNode::setSource(std::unique_ptr<Node> new_node) {
        source = std::move(new_node);
    }
    void StoreNode::setPtr(std::unique_ptr<Node> new_node) {
        ptr = std::move(new_node);
    }
    void StoreNode::accept(NodeVisitor *v) { v->visit(this); }

    PhiNode::PhiNode(std::unique_ptr<Node> to,
                     std::list<std::unique_ptr<Node>> from)
        : to(std::move(to)), from(std::move(from)) {}

    void PhiNode::accept(NodeVisitor *v) { v->visit(this); }

    AsmNode::AsmNode(std::list<std::unique_ptr<lir::Instruction>> assembly)
        : assembly(std::move(assembly)) {}

    std::list<std::unique_ptr<lir::Instruction>> &AsmNode::getAssembly() {
        return assembly;
    }

    void AsmNode::accept(NodeVisitor *v) { v->visit(this); }
} // namespace backend::lir_tree
