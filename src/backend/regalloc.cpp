#include "backend/regalloc.h"
#include "backend/passes/coalesce.h"
#include "backend/passes/coloring.h"
#include "backend/passes/interference.h"
#include "backend/passes/liveness.h"

namespace backend {
    void allocateRegisters(lir::Program &lir) {
        Liveness liveness;
        Interference interference;

        while (true) {
            bool changed = true;
            while (changed) {
                liveness = computeLiveness(lir);
                interference = computeInterference(lir, liveness);

                changed = tryCoalesce(lir, interference);
            }

            auto [can_color, coloring] = tryColor(lir, interference);
            if (can_color) {
                colorRegisters(lir, coloring);
                break;
            }

            // TODO: compute spill costs
            // TODO: spill registers
        }
    }
} // namespace backend
