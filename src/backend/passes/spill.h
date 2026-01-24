#pragma once

#include "backend/lir/lir.h"

namespace backend {
    std::unordered_map<lir::VirtualRegister *, uint64_t>
    computeSpillCosts(lir::Program &lir);

    void spill(lir::Program &lir, lir::VirtualRegister *reg);
    void spillAll(lir::Program &lir);
} // namespace backend
