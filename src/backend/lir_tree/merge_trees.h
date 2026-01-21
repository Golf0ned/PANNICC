#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMerger {
    public:
        TreeMerger(TreeInfo *tree_info, lir::OperandManager *om);
        void
        tryMerge(FunctionTrees &context,
                 std::unordered_map<lir::Register *, std::list<Node *>> &uses);
        void mergeTrees(FunctionTrees &trees);

    private:
        lir::OperandManager *om;
        TreeInfo *tree_info;
    };
} // namespace backend::lir_tree
