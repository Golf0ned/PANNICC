#include <ranges>

#include "middleend/analysis/dominator_tree.h"
#include "middleend/utils/traversal.h"

namespace middleend {

    void DominatorTree::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            TraversalOrderMap traversal_numbers;
            auto traversal_order = numberPostorder(f.get(), traversal_numbers);
            std::ranges::reverse_view reverse_traversal_order{traversal_order};

            auto findLCA = [&](mir::BasicBlock *b1, mir::BasicBlock *b2) {
                while (b1 != b2) {
                    if (traversal_numbers[b1] < traversal_numbers[b2])
                        b1 = immediate_dominators[b1];
                    else
                        b2 = immediate_dominators[b2];
                }
                return b1;
            };

            auto entry = f->getEntryBlock();
            immediate_dominators[entry] = entry;

            bool changed = true;
            while (changed) {
                changed = false;
                for (auto &bb : reverse_traversal_order) {
                    if (bb == entry)
                        continue;

                    mir::BasicBlock *idom = nullptr;
                    for (auto pred : bb->getPredecessors().getEdges()) {
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
