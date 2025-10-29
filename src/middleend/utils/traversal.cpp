#include "middleend/utils/traversal.h"
#include "middleend/mir/mir.h"

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
} // namespace middleend
