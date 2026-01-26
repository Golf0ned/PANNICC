#include "backend/passes/coloring.h"

namespace backend {
    std::pair<bool, RegisterColoring> tryColor(lir::Program &lir,
                                               Interference &interference) {
        int num_regs = 16;

        // TODO: color/"prune(?)" graph
        std::vector<lir::Register *> colorable;
        while (!interference.empty()) {
            // TODO: avoid the infinite
            for (auto &[reg, edges] : interference) {
                if (edges.size() >= num_regs)
                    continue;

                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;

                colorable.push_back(reg);

                for (auto neighbor : edges)
                    interference[neighbor].erase(reg);
                interference.erase(reg);
                break;
            }
        }

        return {true, {}};
    }

    void colorRegisters(lir::Program &lir, RegisterColoring &coloring) {
        // TODO: make changes to lir
    }
} // namespace backend
