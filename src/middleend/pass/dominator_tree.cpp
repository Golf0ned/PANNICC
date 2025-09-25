#include <ranges>

#include "middleend/pass/dominator_tree.h"

namespace middleend {
    void recursePostorder(mir::BasicBlock *bb,
                          std::vector<mir::BasicBlock *> &to,
                          std::unordered_map<mir::BasicBlock *, uint64_t> &tn,
                          uint64_t &counter) {
        tn[bb]; // Prevent loops
        for (auto succ : bb->getSuccessors())
            if (!tn.contains(succ))
                recursePostorder(succ, to, tn, counter);
        tn[bb] = counter++;
        to.push_back(bb);
    }

    void numberPostorder(
        mir::BasicBlock *entry, std::vector<mir::BasicBlock *> &traversal_order,
        std::unordered_map<mir::BasicBlock *, uint64_t> &traversal_numbers) {
        uint64_t counter = 0;
        recursePostorder(entry, traversal_order, traversal_numbers, counter);
    }

    void DominatorTree::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            std::vector<mir::BasicBlock *> traversal_order;
            std::ranges::reverse_view reverse_traversal_order{traversal_order};
            std::unordered_map<mir::BasicBlock *, uint64_t> traversal_numbers;

            auto findLCA = [&](mir::BasicBlock *b1, mir::BasicBlock *b2) {
                while (b1 != b2) {
                    if (traversal_numbers[b1] < traversal_numbers[b2])
                        b1 = immediate_dominators[b1];
                    else
                        b2 = immediate_dominators[b2];
                }
                return b1;
            };

            numberPostorder(f.getEntryBlock(), traversal_order,
                            traversal_numbers);

            immediate_dominators[f.getEntryBlock()] = f.getEntryBlock();
            bool changed = true;
            while (changed) {
                changed = false;
                for (auto &bb : reverse_traversal_order) {
                    if (bb == f.getEntryBlock())
                        continue;

                    mir::BasicBlock *idom = nullptr;
                    for (auto pred : bb->getPredecessors()) {
                        if (immediate_dominators.contains(pred))
                            idom = idom ? findLCA(idom, pred) : pred;
                    }

                    if (idom && idom != immediate_dominators[bb]) {
                        immediate_dominators[bb] = idom;
                        changed = true;
                    }
                }
            }

            for (auto pair : immediate_dominators) {
                immediate_dominated[pair.second].push_back(pair.first);
            }
        }
    }

    // TODO: replace bracket operator with nullptr return
    bool DominatorTree::dominates(mir::BasicBlock *bb1, mir::BasicBlock *bb2) {
        if (immediate_dominators[bb1] == bb1)
            return true;

        auto cur = bb2;
        while (cur != immediate_dominators[cur]) {
            if (cur == bb1)
                return true;
            cur = immediate_dominators[cur];
        }
        return false;
    }

    mir::BasicBlock *DominatorTree::getImmediateDominator(mir::BasicBlock *bb) {
        return immediate_dominators[bb];
    }

    const std::vector<mir::BasicBlock *> &
    DominatorTree::getDominees(mir::BasicBlock *bb) {
        return immediate_dominated[bb];
    }
} // namespace middleend
