#include <iostream>

#include "backend/passes/interference.h"
#include "backend/passes/spill.h"

namespace backend {
    static constexpr uint64_t def_weight = 5;
    static constexpr uint64_t use_weight = 10;
    static constexpr uint64_t max_weight = -1;

    RegisterSet getRestrictedRegisters(lir::Function *f, const Liveness &l,
                                       lir::OperandManager *om) {
        RegisterSet load_only;
        RegisterSet not_load_only;

        size_t i_index = 0;
        auto &gen = l[0];
        // TODO: what in the world
        for (auto &i : f->getInstructions()) {
            auto &gen_i = gen[i_index++];
            auto mov = dynamic_cast<lir::InstructionMov *>(i.get());
            if (mov) {
                auto src = dynamic_cast<lir::Address *>(mov->getSrc());
                auto dst = dynamic_cast<lir::Register *>(mov->getDst());
                if (!src || !dst ||
                    dst->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;

                auto virtual_reg = static_cast<lir::VirtualRegister *>(dst);
                auto flattened_reg =
                    om->getRegister(virtual_reg->getName(), flat_size);
                if (!not_load_only.contains(flattened_reg)) {
                    load_only.insert(flattened_reg);
                    continue;
                }
            }

            for (auto &reg : gen_i) {
                if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                    continue;
                auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
                auto flattened_reg =
                    om->getRegister(virtual_reg->getName(), flat_size);
                if (load_only.contains(flattened_reg)) {
                    load_only.erase(flattened_reg);
                }
            }
        }

        return load_only;
    }

    SpillCosts computeSpillCosts(lir::Function *f, const Liveness &l,
                                 lir::OperandManager *om) {
        SpillCosts spill_costs;
        auto restrictions = getRestrictedRegisters(f, l, om);

        auto add_to_cost = [&](lir::Register *reg, uint64_t amount) {
            if (reg->getRegNum() != lir::RegisterNum::VIRTUAL)
                return;

            auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
            auto flattened_reg =
                om->getRegister(virtual_reg->getName(), flat_size);

            // TODO: regret this choice
            auto &cur_cost = spill_costs[flattened_reg];
            cur_cost = restrictions.contains(flattened_reg) ? -1
                       : cur_cost + amount < cur_cost       ? max_weight
                                                            : cur_cost + amount;
        };

        auto gen = l[0], kill = l[1];
        for (size_t i = 0; i < gen.size(); i++) {
            for (auto &reg : gen[i]) {
                add_to_cost(reg, def_weight);
            }

            for (auto &reg : kill[i]) {
                add_to_cost(reg, use_weight);
            }
        }

        return spill_costs;
    }

    void spill(lir::Function *f, lir::Register *reg, Liveness &l) {
        std::cout << "[Regalloc] Try to spill " << reg->toString() << std::endl;

        auto &gen = l[0], &kill = l[1];

        size_t i_index = 0;
        auto &instructions = f->getInstructions();
        for (auto iter = instructions.begin(); iter != instructions.end();
             iter++) {
            auto &gen_i = gen[i_index], &kill_i = kill[i_index];

            // TODO: check against all sizes of register (64, 32)

            if (gen_i.contains(reg)) {
                // TODO: insert store
                auto store = std::make_unique<lir::InstructionUnknown>();
                instructions.insert(iter, std::move(store));
            }

            if (kill_i.contains(reg)) {
                // TODO: insert load
                auto load = std::make_unique<lir::InstructionUnknown>();
                iter = instructions.insert(std::next(iter), std::move(load));
            }

            i_index++;
        }
    }

    void spillLowestCost(lir::Function *f, const SpillCosts &sc, Liveness &l) {
        uint64_t min_cost = -1;
        auto min_reg = sc.begin()->first;

        for (auto &[reg, cost] : sc) {
            if (cost >= min_cost)
                continue;

            min_cost = cost;
            min_reg = reg;
        }

        spill(f, min_reg, l);
    }
} // namespace backend
