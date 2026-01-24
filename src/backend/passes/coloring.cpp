#include "backend/passes/coloring.h"

namespace backend {
    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference) {
        // TODO: color/"prune(?)" graph
        return {true, {}};
    }

    void colorRegisters(lir::Program &lir, RegisterColoring &coloring) {
        // TODO: make changes to lir
    }
} // namespace backend
