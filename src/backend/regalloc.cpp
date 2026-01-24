#include "backend/regalloc.h"
#include "backend/analysis/interference.h"
#include "backend/analysis/liveness.h"

namespace backend {
    bool tryCoalesce(lir::Program &lir, Interference &interference) {
        // TODO: coalesce registers
        return false;
    }

    void color() {
        // TODO: color/"prune(?)" graph
    }

    bool assignRegisters() {
        // TODO: color graph
        return true;
    }

    void commitRegisters() {
        // TODO: make changes to lir
    }

    void spillRegisters() {
        // TODO: spill and make changes to lir
    }

    void allocateRegisters(lir::Program &lir) {
        Liveness liveness;
        Interference interference;

        while (true) {
            bool changed = true;
            while (changed) {
                changed = false;

                liveness = computeLiveness(lir);
                interference = computeInterference(lir, liveness);

                changed = tryCoalesce(lir, interference);
            }

            // TODO: compute spill costs
            color();

            if (assignRegisters()) {
                commitRegisters();
                break;
            }

            spillRegisters();
        }
    }
} // namespace backend
