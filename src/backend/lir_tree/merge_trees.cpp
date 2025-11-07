#include "backend/lir_tree/merge_trees.h"

namespace backend::lir_tree {
    std::list<Tree> TreeMerger::getResult() { return std::move(merged_trees); }

    void TreeMerger::mergeTrees(std::list<Tree> &trees) {
        // Given trees T1 and T2, and T1 uses V defined by T2
        // Merge T2 into T1 if:
        // - V is dead after T1 OR V is only used by T1
        // - No AsmNode between T1 and T2
        // - No instructions in [T2, T1) that depend on T2
        //   - If V is variable: No uses in range
        //   - If V is variable: No defs of variables used by T2 in range
        //   - If V is mem access: No memory instruction in range

        std::unordered_map<Node *, std::vector<Tree &>> uses;

        std::list<Tree> cur;

        auto iter = trees.end();
        while (iter != trees.begin()) {
            --iter;

            auto asm_tree = dynamic_cast<AsmNode *>(iter->getRoot().get());
            if (!asm_tree) {
                // update uses
                // move to cur if not asm node
                continue;
            }

            // merge if asm node
            // helper function?
            bool changed = true;
            while (changed) {
                changed = false;
                // do the mergin
            }
            uses.clear();
            merged_trees.splice(merged_trees.end(), trees);
            // splice the asm node onto merged_trees
        }

        // try merge one more time
    }

} // namespace backend::lir_tree
