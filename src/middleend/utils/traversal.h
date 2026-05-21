#pragma once

#include "middleend/mir/mir.h"

namespace middleend {

using TraversalOrderMap = std::unordered_map<mir::BasicBlock *, uint64_t>;

// TODO: make iterators

std::list<mir::BasicBlock *> traversePreorder(mir::FunctionDefinition *f);

std::list<mir::BasicBlock *> numberPostorder(mir::FunctionDefinition *f,
                                             TraversalOrderMap &tom);
std::list<mir::BasicBlock *> traversePostorder(mir::FunctionDefinition *f);

std::list<mir::BasicBlock *> traverseTraces(mir::FunctionDefinition *f);

} // namespace middleend
