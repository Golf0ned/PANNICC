#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMergeVisitor : public NodeVisitor {
    public:
        std::list<std::list<Tree>> getResult();

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
        std::list<Tree> current_trees;
        std::list<std::list<Tree>> merged_trees;
    };
} // namespace backend::lir_tree
