#include <iostream>

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
                spill_costs[reg] += use_weight;
            }

            for (auto &reg : kill[i]) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                spill_costs[reg] += def_weight;
            }
        }

        return spill_costs;
    }

    void spill(lir::Function *f, lir::Register *reg) {
        // TODO: spill and make changes to lir
        std::cout << "[Regalloc] Try to spill " << reg->toString() << std::endl;
    }

    void spillLowestCost(lir::Function *f, const SpillCosts &sc) {
        uint64_t min_cost = -1;
        auto min_reg = sc.begin()->first;

        for (auto &[reg, cost] : sc) {
            if (cost >= min_cost)
                continue;

            min_cost = cost;
            min_reg = reg;
        }

        spill(f, min_reg);
    }
} // namespace backend
