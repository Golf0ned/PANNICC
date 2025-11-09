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

                // TODO: think about how we can even compare things
                auto t1_iter = cur.begin();
                while (t1_iter != cur.end()) {
                    auto t1 = t1_iter->get();
                    for (auto &t2 : trees.getLeaves(t1)) {
                        auto &t2_uses = uses[t2.get()];
                        if (t2_uses.back() != t1)
                            continue;

                        t2_uses.pop_back();
                        if (t2_uses.size() > 1 &&
                            *std::prev(t2_uses.end(), 2) == t1)
                            continue;

                        auto t2_iter = t1_iter;
                        bool t2_seen = false;
                        while (!t2_seen) {
                            auto cur_tree = *(++t2_iter);
                            t2_seen = t2 == cur_tree;

                            auto cur_uses_t2 = [&]() {
                                for (auto cur_leaf :
                                     trees.getLeaves(cur_tree.get())) {
                                    if (cur_leaf == t2)
                                        return false;
                                }
                                return true;
                            };

                            auto cur_is_t2_leaf = [&]() {
                                for (auto t2_leaf : trees.getLeaves(t2.get())) {
                                    if (t2_leaf == cur_tree)
                                        return false;
                                }
                                return true;
                            };

                            auto maybe_aliases_memory = [&]() {
                                return trees.hasMemoryInstruction(t2.get()) &&
                                       trees.hasMemoryInstruction(
                                           cur_tree.get());
                            };

                            // clang-format off
                            if (false
                                || cur_uses_t2()
                                || cur_is_t2_leaf()
                                || maybe_aliases_memory()
                            ) {
                                // clang-format on
                                continue;
                            }

                            // TODO: merge
                        }
                    }

                    t1_iter++;
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
