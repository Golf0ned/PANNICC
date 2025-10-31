#pragma once

#include "middleend/mir/mir.h"

namespace middleend {
    using TraversalOrderMap = std::unordered_map<mir::BasicBlock *, uint64_t>;

    // TODO: make iterators

    std::list<mir::BasicBlock *> traversePreorder(mir::Function *f);

    std::list<mir::BasicBlock *> numberPostorder(mir::Function *f,
                                                 TraversalOrderMap &tom);
    std::list<mir::BasicBlock *> traversePostorder(mir::Function *f);

    std::list<mir::BasicBlock *> traverseTraces(mir::Function *f);
} // namespace middleend
