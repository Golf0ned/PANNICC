#include "backend/regalloc.h"
#include "backend/passes/coalesce.h"
#include "backend/passes/coloring.h"
#include "backend/passes/interference.h"
#include "backend/passes/liveness.h"
#include "backend/passes/spill.h"

namespace backend {
    void allocateRegisters(lir::Program &lir) {
        auto om = lir.getOm();

        for (auto &unique_f : lir.getFunctions()) {
            auto f = unique_f.get();

            Liveness liveness;
            Interference interference;

            while (true) {
                bool changed = true;
                while (changed) {
                    liveness = computeLiveness(f, om);
                    interference = computeInterference(f, liveness, om);

                    changed = tryCoalesce(f, interference);
                }

                auto spill_costs = computeSpillCosts(liveness);

                auto [can_color, coloring] =
                    tryColor(f, interference, spill_costs, om);
                if (can_color) {
                    assignRegisters(f, coloring, om);
                    break;
                }

                spillLowestCost(f, spill_costs);
            }
        }
    }
} // namespace backend
