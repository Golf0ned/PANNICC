#include "backend/lir_tree/merge_trees.h"

namespace backend::lir_tree {
    std::list<Tree> TreeMerger::getResult() { return std::move(merged_trees); }

    void TreeMerger::consumeTree(Tree &t) {
        current_trees.push_back(std::move(t));

        auto asm_tree = dynamic_cast<AsmNode *>(t.getRoot().get());
        if (asm_tree)
            mergeTrees();
    }

    void TreeMerger::mergeTrees() {
        // TODO: do merging
        merged_trees.splice(merged_trees.end(), current_trees);
    }

} // namespace backend::lir_tree
