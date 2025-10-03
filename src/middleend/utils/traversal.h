#pragma once

#include "middleend/mir/mir.h"

namespace middleend {
    // TODO: preorder basic block iterator
    // TODO: reverse postorder basic block iterator

    void recursePostorder(mir::BasicBlock *bb, std::list<mir::BasicBlock *> &to,
                          std::unordered_map<mir::BasicBlock *, uint64_t> &tn,
                          uint64_t &counter);

    void numberPostorder(
        mir::BasicBlock *entry, std::list<mir::BasicBlock *> &traversal_order,
        std::unordered_map<mir::BasicBlock *, uint64_t> &traversal_numbers);
} // namespace middleend
