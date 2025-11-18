#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMerger {
    public:
        std::list<std::unique_ptr<Node>> getResult();

        void mergeTrees(Forest &trees, lir::OperandManager &om);

    private:
        std::list<std::unique_ptr<Node>> function_trees;
        std::list<std::unique_ptr<Node>> program_trees;
    };
} // namespace backend::lir_tree
