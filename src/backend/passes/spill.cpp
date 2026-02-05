#include <iostream>

#include "backend/passes/interference.h"
#include "backend/passes/spill.h"

namespace backend {
    static constexpr uint64_t def_weight = 5;
    static constexpr uint64_t use_weight = 10;
    static constexpr uint64_t copy_weight = 5;
    static constexpr uint64_t max_weight = -1;

    void addToCost(uint64_t &cur_cost, uint64_t to_add) {
        cur_cost =
            cur_cost + to_add < cur_cost ? max_weight : cur_cost + to_add;
    }

    SpillCosts computeSpillCosts(const Liveness &l, lir::OperandManager *om) {
        SpillCosts spill_costs;

        auto gen = l[0], kill = l[1];
        for (size_t i = 0; i < gen.size(); i++) {
            for (auto &reg : gen[i]) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
                auto color = om->getRegister(virtual_reg->getName(), flat_size);

                addToCost(spill_costs[color], use_weight);
            }

            for (auto &reg : kill[i]) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
                auto color = om->getRegister(virtual_reg->getName(), flat_size);

                addToCost(spill_costs[color], use_weight);
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
