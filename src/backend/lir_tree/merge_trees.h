#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMerger {
    public:
        std::list<Tree> getResult();

        void consumeTree(Tree &t);
        void mergeTrees();

    private:
        std::list<Tree> current_trees;
        std::list<Tree> merged_trees;
    };
} // namespace backend::lir_tree
