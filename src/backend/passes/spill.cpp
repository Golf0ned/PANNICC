#include "backend/passes/spill.h"

namespace backend {
    static constexpr uint64_t def_weight = 5;
    static constexpr uint64_t use_weight = 5;
    static constexpr uint64_t copy_weight = 5;

    SpillCosts computeSpillCosts(const Liveness &l) {
        SpillCosts spill_costs;

        auto gen = l[0], kill = l[1];
        for (size_t i = 0; i < gen.size(); i++) {
            for (auto &reg : gen[i]) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
                spill_costs[virtual_reg] += use_weight;
            }

            for (auto &reg : kill[i]) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
                spill_costs[virtual_reg] += def_weight;
            }
        }

        return spill_costs;
    }

    void spill(lir::Function *f, lir::VirtualRegister *reg) {
        // TODO: spill and make changes to lir
    }
} // namespace backend
