#pragma once

#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    class TreeMerger {
    public:
        std::list<Tree> getResult();

        void mergeTrees(std::list<Tree> &trees);

    private:
        std::list<Tree> merged_trees;
    };
} // namespace backend::lir_tree
