#include "backend/regalloc.h"
#include "backend/passes/coalesce.h"
#include "backend/passes/coloring.h"
#include "backend/passes/interference.h"
#include "backend/passes/liveness.h"
#include "backend/passes/spill.h"

namespace backend {
    void allocateRegisters(lir::Program &lir) {
        auto om = lir.getOm();

        for (auto &f : lir.getFunctions()) {
            auto f_ptr = f.get();

            Liveness liveness;
            Interference interference;

            while (true) {
                bool changed = true;
                while (changed) {
                    liveness = computeLiveness(f_ptr, om);
                    interference = computeInterference(f_ptr, liveness, om);

                    changed = tryCoalesce(f_ptr, interference);
                }

                auto spill_costs = computeSpillCosts(liveness);

                auto [can_color, coloring] =
                    tryColor(f_ptr, interference, spill_costs, om);
                if (can_color) {
                    assignRegisters(f_ptr, coloring, om);
                    break;
                }

                // TODO: spill lowest cost register
            }
        }
    }
} // namespace backend
