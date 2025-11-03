#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    TreeTileVisitor::TreeTileVisitor(lir::OperandManager &om) : om(om) {}

    std::list<std::unique_ptr<lir::Instruction>> TreeTileVisitor::getResult() {
        return std::move(assembly);
    }

    void TreeTileVisitor::visit(Node *n) {}

    void TreeTileVisitor::visit(RegisterNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(ImmediateNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(OpNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(AllocaNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(LoadNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(StoreNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(PhiNode *n) {
        // TODO
    }

    void TreeTileVisitor::visit(AsmNode *n) {
        assembly.splice(assembly.end(), n->getAssembly());
    }
} // namespace backend::lir_tree
