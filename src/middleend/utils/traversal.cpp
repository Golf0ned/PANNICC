#include "middleend/utils/traversal.h"

namespace middleend {
    void recursePostorder(mir::BasicBlock *bb, std::list<mir::BasicBlock *> &to,
                          std::unordered_map<mir::BasicBlock *, uint64_t> &tn,
                          uint64_t &counter) {
        tn[bb]; // Prevent loops
        for (auto [succ, _] : bb->getSuccessors())
            if (!tn.contains(succ))
                recursePostorder(succ, to, tn, counter);
        tn[bb] = counter++;
        to.push_back(bb);
    }

    void numberPostorder(
        mir::BasicBlock *entry, std::list<mir::BasicBlock *> &traversal_order,
        std::unordered_map<mir::BasicBlock *, uint64_t> &traversal_numbers) {
        uint64_t counter = 0;
        recursePostorder(entry, traversal_order, traversal_numbers, counter);
    }
} // namespace middleend
