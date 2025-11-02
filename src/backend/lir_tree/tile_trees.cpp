#include "backend/lir_tree/tile_trees.h"

namespace backend::lir_tree {
    TreeTileVisitor::TreeTileVisitor(lir::OperandManager &om) : om(om) {}

    std::list<std::unique_ptr<lir::Instruction>> TreeTileVisitor::getResult() {
        return std::move(assembly);
    }

    void TreeTileVisitor::visit(Node *n) {}

    void TreeTileVisitor::visit(RegisterNode *n) {}

    void TreeTileVisitor::visit(ImmediateNode *n) {}

    void TreeTileVisitor::visit(OpNode *n) {}

    void TreeTileVisitor::visit(AllocaNode *n) {}

    void TreeTileVisitor::visit(LoadNode *n) {}

    void TreeTileVisitor::visit(StoreNode *n) {}

    void TreeTileVisitor::visit(PhiNode *n) {}

    void TreeTileVisitor::visit(AsmNode *n) {
        assembly.splice(assembly.end(), n->getAssembly());
    }
} // namespace backend::lir_tree
