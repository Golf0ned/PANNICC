#include <algorithm>
#include <cassert>
#include <iostream>

#include "backend/lir_tree/merge_trees.h"
#include "backend/lir_tree/node.h"

namespace backend::lir_tree {
    std::list<std::shared_ptr<Node>> TreeMerger::getResult() {
        merged_trees.reverse();
        return std::move(merged_trees);
    }

    void TreeMerger::mergeTrees(Forest &trees, lir::OperandManager &om) {
        std::unordered_map<std::string, std::list<Node *>> uses;
        std::list<std::shared_ptr<Node>> context;

        auto try_merge = [&]() {
            bool changed = true;
            while (changed) {
                changed = false;

                auto t1_iter = context.begin();
                while (t1_iter != context.end()) {
                    auto t1 = t1_iter->get();
                    auto t1_reg = dynamic_cast<RegisterNode *>(t1);
                    if (!t1_reg)
                        continue;

                    auto &t1_leaves = trees.getLeaves(t1);
                    for (auto t2_iter = t1_leaves.begin();
                         t2_iter != t1_leaves.end(); t2_iter++) {
                        auto t2 = t2_iter->get();
                        auto t2_reg = dynamic_cast<RegisterNode *>(t2);
                        if (!t2_reg)
                            continue;

                        auto &t2_uses = uses[t2_reg->getName()];
                        if (t2_uses.back() != t1)
                            continue;

                        if (t2_uses.size() > 1 &&
                            t2_uses.back() == *std::prev(t2_uses.end(), 2))
                            continue;

                        auto cur_iter = t1_iter;
                        bool t2_seen = false;
                        while (!t2_seen) {
                            auto cur = (++cur_iter)->get();
                            auto cur_reg = dynamic_cast<RegisterNode *>(cur);

                            // Cannot merge if cur is a use of t2
                            for (auto cur_leaf : trees.getLeaves(cur)) {
                                auto cur_leaf_reg =
                                    dynamic_cast<RegisterNode *>(
                                        cur_leaf.get());
                                if (cur_leaf_reg &&
                                    t2_reg->sameReg(cur_leaf_reg))
                                    goto cannot_merge;
                            }

                            // Cannot merge if cur defines a reg used by t2
                            if (cur_reg) {
                                for (auto t2_leaf : trees.getLeaves(t2)) {
                                    auto t2_leaf_reg =
                                        dynamic_cast<RegisterNode *>(
                                            t2_leaf.get());
                                    if (t2_leaf_reg &&
                                        t2_leaf_reg->sameReg(cur_reg))
                                        goto cannot_merge;
                                }
                            }

                            if (trees.hasMemoryInstruction(t2) &&
                                trees.hasMemoryInstruction(cur))
                                goto cannot_merge;

                            t2_seen = t2_reg->sameReg(cur_reg);
                        }

                    cannot_merge:

                        if (!t2_seen)
                            continue;

                        std::cout << "merging" << std::endl;

                        auto cur = cur_iter->get();
                        auto cur_reg = static_cast<RegisterNode *>(cur);

                        t2_reg->consume(cur_reg);

                        t1_leaves.erase(std::find(t1_leaves.begin(),
                                                  t1_leaves.end(), *t2_iter));
                        for (auto &leaf : trees.getLeaves(cur_reg))
                            t1_leaves.push_back(std::move(leaf));

                        if (trees.hasMemoryInstruction(t1) &&
                            trees.hasMemoryInstruction(t2))
                            trees.propagateMemoryInstruction(t1);

                        for (auto &cur_leaf : trees.getLeaves(cur)) {
                            auto cur_leaf_reg =
                                dynamic_cast<RegisterNode *>(cur);
                            if (!cur_leaf_reg)
                                continue;
                            auto cur_leaf_uses = uses[cur_leaf_reg->getName()];
                            assert(cur_leaf_uses.back() == cur_reg);
                            cur_leaf_uses.pop_back();
                            cur_leaf_uses.push_back(t1);
                        }

                        context.erase(std::find(context.begin(), context.end(),
                                                *cur_iter));
                        changed = true;
                    }

                    t1_iter++;
                }
            }
        };

        while (!trees.empty()) {
            auto tree = trees.pop();

            auto asm_tree = dynamic_cast<AsmNode *>(tree.get());
            if (!asm_tree) {
                for (auto &leaf : trees.getLeaves(tree.get())) {
                    auto reg_ptr = dynamic_cast<RegisterNode *>(leaf.get());
                    if (reg_ptr)
                        uses[reg_ptr->getName()].push_front(tree.get());
                }
                context.push_back(std::move(tree));
                continue;
            }

            try_merge();
            merged_trees.splice(merged_trees.end(), context);

            merged_trees.push_back(std::move(tree));
            uses.clear();
        }

        // try merge one more time
        try_merge();
        merged_trees.splice(merged_trees.end(), context);
    }
} // namespace backend::lir_tree
