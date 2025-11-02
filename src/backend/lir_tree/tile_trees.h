#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeTileVisitor : public NodeVisitor {
    public:
        TreeTileVisitor(lir::OperandManager &om);

        std::list<std::unique_ptr<lir::Instruction>> getResult();

        virtual void visit(Node *n);
        virtual void visit(RegisterNode *n);
        virtual void visit(ImmediateNode *n);
        virtual void visit(OpNode *n);
        virtual void visit(AllocaNode *n);
        virtual void visit(LoadNode *n);
        virtual void visit(StoreNode *n);
        virtual void visit(PhiNode *n);
        virtual void visit(AsmNode *n);

    private:
        lir::OperandManager &om;
        std::list<std::unique_ptr<lir::Instruction>> assembly;
    };
} // namespace backend::lir_tree
