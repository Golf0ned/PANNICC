#include <algorithm>
#include <cassert>

#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    TreeMerger::TreeMerger(TreeInfo *tree_info, lir::OperandManager *om)
        : tree_info(tree_info), om(om) {}

    void TreeMerger::tryMerge(
        FunctionTrees &context,
        std::unordered_map<lir::Register *, std::list<Node *>> &uses) {
        bool changed = true;
        while (changed) {
            changed = false;

            // Reverse iteration
            for (auto t1_iter = context.begin(); t1_iter != context.end();
                 t1_iter++) {
                auto t1 = t1_iter->get();

                auto &t1_leaves = tree_info->getLeaves(t1);
                for (auto t2_iter = t1_leaves.begin();
                     t2_iter != t1_leaves.end(); t2_iter++) {
                    auto t2 = *t2_iter;
                    auto t2_reg = dynamic_cast<RegisterNode *>(t2);
                    if (!t2_reg)
                        continue;

                    auto t2_uses_iter = uses.find(t2_reg->getReg());
                    if (t2_uses_iter == uses.end())
                        continue;

                    auto &t2_uses = t2_uses_iter->second;
                    if (t2_uses.empty() || t2_uses.back() != t1)
                        continue;

                    if (t2_uses.size() > 1 &&
                        t2_uses.back() == *std::prev(t2_uses.end(), 2))
                        continue;

                    auto cur_iter = t1_iter;
                    bool t2_seen = false;
                    while (!t2_seen) {
                        auto cur = (++cur_iter)->get();
                        if (cur_iter == context.end())
                            break;

                        auto cur_reg = dynamic_cast<RegisterNode *>(cur);

                        auto cur_uses_t2 = [&]() {
                            for (auto &cur_leaf : tree_info->getLeaves(cur)) {
                                auto cur_leaf_reg =
                                    dynamic_cast<RegisterNode *>(cur_leaf);
                                if (cur_leaf_reg &&
                                    t2_reg->sameReg(cur_leaf_reg)) {
                                    return true;
                                }
                            }
                            return false;
                        };

                        auto cur_is_t2_leaf = [&]() {
                            if (!cur_reg)
                                return false;
                            for (auto &t2_leaf : tree_info->getLeaves(t2)) {
                                auto t2_leaf_reg =
                                    dynamic_cast<RegisterNode *>(t2_leaf);
                                if (t2_leaf_reg &&
                                    t2_leaf_reg->sameReg(cur_reg)) {
                                    return true;
                                }
                            }
                            return false;
                        };

                        auto maybe_aliases = [&]() {
                            return tree_info->hasMemInst(t1) &&
                                   tree_info->hasMemInst(cur);
                        };

                        // clang-format off
                            if (false
                                || cur_uses_t2()
                                || cur_is_t2_leaf()
                                || maybe_aliases()
                            ) {
                            // clang-format on
                            break;
                        }

                        if (!cur_reg)
                            continue;

                        t2_seen = t2_reg->sameReg(cur_reg);
                        if (!t2_seen)
                            continue;

                        // Transfer info from cur to t2
                        t2_reg->consume(cur_reg);

                        // Transfer uses from cur to t1
                        for (auto &cur_leaf : tree_info->getLeaves(cur)) {
                            auto cur_leaf_reg =
                                dynamic_cast<RegisterNode *>(cur_leaf);
                            if (!cur_leaf_reg)
                                continue;
                            auto &cur_leaf_uses = uses[cur_leaf_reg->getReg()];
                            *std::find(cur_leaf_uses.begin(),
                                       cur_leaf_uses.end(), cur) = t1;
                        }
                        uses.erase(t2_reg->getReg());

                        // Transfer info in forest from cur to t1
                        for (auto &new_leaf : tree_info->getLeaves(cur))
                            t1_leaves.push_back(std::move(new_leaf));
                        auto old_leaf =
                            std::find(t1_leaves.begin(), t1_leaves.end(), t2);
                        t1_leaves.erase(old_leaf);

                        if (tree_info->hasMemInst(t1) ||
                            tree_info->hasMemInst(cur))
                            tree_info->setMemInst(t1);

                        // Remove cur from context
                        context.remove(*cur_iter);

                        changed = true;
                        t2_iter = t1_leaves.begin();
                        break;
                    }
                }
            }
        }
    }

    void TreeMerger::mergeTrees(FunctionTrees &trees) {
        FunctionTrees merged;

        FunctionTrees context;
        std::unordered_map<lir::Register *, std::list<Node *>> uses;

        while (!trees.empty()) {
            auto tree = std::move(trees.back());
            trees.pop_back();

            auto asm_tree = dynamic_cast<AsmNode *>(tree.get());
            if (!asm_tree) {
                for (auto &leaf : tree_info->getLeaves(tree.get())) {
                    auto leaf_reg = dynamic_cast<RegisterNode *>(leaf);
                    if (leaf_reg)
                        uses[leaf_reg->getReg()].push_front(tree.get());
                }
                context.push_back(std::move(tree));
                continue;
            }

            tryMerge(context, uses);
            merged.splice(merged.end(), context);
            merged.push_back(std::move(tree));
            // uses.clear();
        }

        // try merge one more time
        tryMerge(context, uses);
        merged.splice(merged.end(), context);

        merged.reverse();
        trees = std::move(merged);
    }
} // namespace backend::lir_tree
