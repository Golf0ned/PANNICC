#include "backend/passes/coloring.h"

namespace backend {
    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference) {
        int num_regs = 16;

        std::vector<lir::Register *> colorable;
        bool changed = true;
        while (changed) {
            changed = false;

            for (auto &[reg, edges] : interference) {
                if (edges.size() >= num_regs)
                    continue;

                colorable.push_back(reg);

                for (auto neighbor : edges)
                    interference[neighbor].erase(reg);
                interference.erase(reg);

                changed = true;
                break;
            }

            if (!interference.empty()) {
                // TODO: find node with min spill cost divided by degree, push
                // onto stack

                // For now, we just stick topmost node on the stack lol
                colorable.push_back(interference.begin()->first);

                changed = true;
            }
        }

        // TODO: assign registers

        return {true, {}};
    }

    void colorRegisters(lir::Program &lir, RegisterColoring &coloring) {
        // TODO: make changes to lir
    }
} // namespace backend
