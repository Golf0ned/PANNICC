#pragma once

#include "backend/lir/lir.h"
#include "backend/passes/liveness.h"

namespace backend {
    using SpillCosts = std::unordered_map<lir::Register *, uint64_t>;

    SpillCosts computeSpillCosts(lir::Function *f, const Liveness &l,
                                 lir::OperandManager *om);

    void spill(lir::Function *f, lir::VirtualRegister *reg, Liveness &l);
    void spillLowestCost(lir::Function *f, const SpillCosts &sc, Liveness &l);
} // namespace backend
