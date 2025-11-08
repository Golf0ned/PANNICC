#include "backend/lir_tree/merge_trees.h"

namespace backend::lir_tree {
    std::list<std::shared_ptr<Node>> TreeMerger::getResult() {
        return std::move(merged_trees);
    }

    void TreeMerger::mergeTrees(Forest &trees) {
        std::unordered_map<Node *, std::list<Node *>> uses;
        std::list<std::shared_ptr<Node>> cur;

        auto try_merge = [&]() {
            bool changed = true;
            while (changed) {
                changed = false;

                for (auto iter = cur.begin(); iter != cur.end(); iter++) {
                    auto tree = iter->get();
                    for (auto &leaf : trees.getLeaves(tree)) {
                        auto &uses_stack = uses[leaf.get()];
                        if (uses_stack.back() != tree)
                            continue;

                        // TODO: iterate from (tree, leaf]
                        bool can_merge = true;
                        auto inner_iter = iter;
                        while (inner_iter->get() != leaf.get()) {
                            // TODO: skip if cur uses leaf
                            // TODO: skip if cur is a leaf of a leaf
                            // TODO: skip if mem instruction and leaf has mem
                            //       instruction
                        }

                        if (can_merge) {
                            // TODO: do merge
                        }
                    }
                }
            }
        };

        while (!trees.empty()) {
            auto tree = trees.pop();

            auto asm_tree = dynamic_cast<AsmNode *>(tree.get());
            if (!asm_tree) {
                for (auto &node : trees.getLeaves(tree.get()))
                    uses[node.get()].push_front(tree.get());
                cur.push_back(std::move(tree));
                continue;
            }

            try_merge();
            merged_trees.splice(merged_trees.end(), cur);
            uses.clear();
        }

        // try merge one more time
        try_merge();
        merged_trees.splice(merged_trees.end(), cur);
    }
} // namespace backend::lir_tree
