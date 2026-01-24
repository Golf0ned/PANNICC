#pragma once

#include "backend/lir/operand.h"
#include "backend/passes/interference.h"

namespace backend {
    using RegisterColoring =
        std::unordered_map<lir::VirtualRegister *, lir::Register *>;

    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference);
    void colorRegisters(lir::Program &lir, RegisterColoring &coloring);
} // namespace backend
