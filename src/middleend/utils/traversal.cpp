#include <deque>

#include "middleend/mir/mir.h"
#include "middleend/utils/traversal.h"

namespace middleend {
    std::list<mir::BasicBlock *> traversePreorder(mir::Function *f) {
        // TODO: implement
        std::list<mir::BasicBlock *> traversal_order;
        return traversal_order;
    }

    void recursePostorder(mir::BasicBlock *bb, std::list<mir::BasicBlock *> &to,
                          TraversalOrderMap &tn, uint64_t &counter) {
        tn[bb]; // Prevent loops
        for (auto succ : bb->getSuccessors().getUniqueEdges())
            if (!tn.contains(succ))
                recursePostorder(succ, to, tn, counter);
        tn[bb] = counter++;
        to.push_back(bb);
    }

    std::list<mir::BasicBlock *> numberPostorder(mir::Function *f,
                                                 TraversalOrderMap &tom) {
        std::list<mir::BasicBlock *> traversal_order;

        // TODO: rewrite iterative
        uint64_t counter = 0;
        recursePostorder(f->getEntryBlock(), traversal_order, tom, counter);

        return traversal_order;
    }

    std::list<mir::BasicBlock *> traversePostorder(mir::Function *f) {
        TraversalOrderMap tom;
        return numberPostorder(f, tom);
    }

    std::list<mir::BasicBlock *> traverseLeastBranches(mir::Function *f) {
        // TODO: implement
        std::list<mir::BasicBlock *> traversal_order;
        return traversal_order;
    }

    std::list<mir::BasicBlock *> traverseTraces(mir::Function *f) {
        std::list<mir::BasicBlock *> traversal_order;

        auto entry = f->getEntryBlock();
        std::unordered_set<mir::BasicBlock *> visited = {};
        std::deque<mir::BasicBlock *> worklist = {entry};

        while (!worklist.empty()) {
            auto cur = worklist.front();
            worklist.pop_front();

            if (visited.contains(cur))
                continue;

            // Create trace, starting at cur
            while (cur) {
                traversal_order.push_back(cur);
                visited.insert(cur);

                mir::BasicBlock *next = nullptr;
                for (auto succ : cur->getSuccessors().getEdges()) {
                    if (visited.contains(succ))
                        continue;

                    if (next)
                        worklist.push_back(succ);
                    else
                        next = succ;
                }
                cur = next;
            }
        }

        return traversal_order;
    }
} // namespace middleend
