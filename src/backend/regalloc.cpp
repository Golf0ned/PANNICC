#include "backend/regalloc.h"
#include "backend/passes/coalesce.h"
#include "backend/passes/coloring.h"
#include "backend/passes/interference.h"
#include "backend/passes/liveness.h"

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

                auto [can_color, coloring] = tryColor(f_ptr, interference, om);
                if (can_color) {
                    colorRegisters(f_ptr, coloring, om);
                    break;
                }

                // TODO: compute spill costs
                // TODO: spill registers
            }
        }
    }
} // namespace backend
