#include "backend/lir_tree/merge_trees.h"

namespace backend::lir_tree {
    std::list<Tree> TreeMerger::getResult() { return std::move(merged_trees); }

    void TreeMerger::mergeTrees(std::list<Tree> &trees) {
        std::unordered_map<Node *, Tree &> last_def;

        // Given trees T1 and T2, and T1 uses V defined by T2
        // Merge T2 into T1 if:
        // - V is dead after T1 OR V is only used by T1
        // - No instructions in [T2, T1) that depend on T2
        //   - If V is variable: No uses in range
        //   - If V is variable: No defs of variables used by T2 in range
        //   - If V is mem access: No memory instruction in range

        bool changed = true;
        while (changed) {
            changed = false;

            auto iter = trees.end();
            while (iter != trees.begin()) {
                iter--;
            }
        }

        merged_trees.splice(merged_trees.end(), trees);
    }

} // namespace backend::lir_tree
