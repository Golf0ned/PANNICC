#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMerger {
    public:
        std::list<std::shared_ptr<Node>> getResult();

        void mergeTrees(Forest &trees, lir::OperandManager &om);

    private:
        std::list<std::shared_ptr<Node>> merged_trees;
    };
} // namespace backend::lir_tree
