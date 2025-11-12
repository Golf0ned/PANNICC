#include <algorithm>
#include <cassert>

#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    std::string regToString(Node *node) {
        auto reg = static_cast<RegisterNode *>(node);
        return reg->getName();
    }

    bool sameReg(std::shared_ptr<Node> &t1, std::shared_ptr<Node> &t2) {
        auto t1_reg = dynamic_cast<RegisterNode *>(t1.get());
        if (!t1_reg)
            return false;

        auto t2_reg = dynamic_cast<RegisterNode *>(t2.get());
        if (!t2_reg)
            return false;

        return t1_reg->sameReg(t2_reg);
    }

    void consume(std::shared_ptr<Node> &from, std::shared_ptr<Node> &to) {}

    std::list<std::shared_ptr<Node>> TreeMerger::getResult() {
        return std::move(merged_trees);
    }

    void TreeMerger::mergeTrees(Forest &trees) {
        std::unordered_map<std::string, std::list<Node *>> uses;
        std::list<std::shared_ptr<Node>> cur;

        auto try_merge = [&]() {
            bool changed = true;
            while (changed) {
                changed = false;

                auto t1_iter = cur.begin();
                while (t1_iter != cur.end()) {
                    auto t1 = t1_iter->get();
                    for (auto &t2 : trees.getLeaves(t1)) {
                        auto &t2_uses = uses[regToString(t2.get())];
                        if (t2_uses.back() != t1)
                            continue;

                        if (t2_uses.size() > 1 &&
                            t2_uses.back() == *std::prev(t2_uses.end(), 2))
                            continue;

                        auto t2_iter = t1_iter;
                        bool t2_seen = false;
                        while (!t2_seen) {
                            auto &cur_tree = *(++t2_iter);
                            t2_seen = sameReg(t2, cur_tree);

                            auto cur_uses_t2 = [&]() {
                                for (auto cur_leaf :
                                     trees.getLeaves(cur_tree.get())) {
                                    if (sameReg(cur_leaf, t2))
                                        return false;
                                }
                                return true;
                            };

                            auto cur_is_t2_leaf = [&]() {
                                for (auto t2_leaf : trees.getLeaves(t2.get())) {
                                    if (sameReg(t2_leaf, cur_tree))
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

                            auto t2_reg = static_cast<RegisterNode *>(t2.get());
                            auto cur_reg =
                                static_cast<RegisterNode *>(cur_tree.get());
                            t2_reg->consume(cur_reg);

                            auto &t1_leaves = trees.getLeaves(t1);
                            t1_leaves.erase(std::find(t1_leaves.begin(),
                                                      t1_leaves.end(), t2));
                            for (auto &leaf : trees.getLeaves(cur_reg))
                                t1_leaves.push_back(std::move(leaf));

                            if (trees.hasMemoryInstruction(t1) &&
                                trees.hasMemoryInstruction(t2.get()))
                                trees.propagateMemoryInstruction(t1);

                            for (auto &leaf : trees.getLeaves(cur_reg)) {
                                auto leaf_uses = uses[regToString(leaf.get())];
                                assert(leaf_uses.back() == cur_reg);
                                leaf_uses.pop_back();
                                leaf_uses.push_back(t1);
                            }

                            cur.erase(
                                std::find(cur.begin(), cur.end(), cur_tree));
                            changed = true;
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
                for (auto &leaf : trees.getLeaves(tree.get()))
                    uses[regToString(leaf.get())].push_front(tree.get());
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
