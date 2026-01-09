#include "backend/analysis/interference.h"

namespace backend {
    Interference::Interference(lir::Program &p) : program(p) {}

    void Interference::addEdge(lir::Register *first, lir::Register *second) {
        auto first_idx = reg_to_index.at(first),
             second_idx = reg_to_index.at(second);

        adj_matrix[first_idx][second_idx] = true;
        adj_matrix[second_idx][first_idx] = true;
    }

    void Interference::computeInterference(Liveness &liveness) {
        auto om = program.getOm();

        auto num_regs = om->getNumRegisters();
        adj_matrix = std::vector<std::vector<bool>>(
            num_regs, std::vector<bool>(num_regs, false));
        reg_to_index.clear();

        for (auto reg : lir::getAllRegisters()) {
            auto new_reg = om->getRegister(reg);
            for (auto &[prev_reg, _] : reg_to_index)
                addEdge(prev_reg, new_reg);
            reg_to_index[new_reg] = reg_to_index.size();
        }

        auto out = liveness.getOut();
        // TODO: write gen getter
        // auto gen = liveness.getGen();
        std::vector<RegisterSet> gen(out.size());

        for (size_t i = 0; i < out.size(); i++) {
            auto &gen_i = gen[i], &out_i = out[i];
            for (auto gen_reg : gen_i) {
                // TODO: make this not ugly thanks
                for (auto prev_gen_reg : gen_i) {
                    if (gen_reg == prev_gen_reg)
                        break;
                    addEdge(gen_reg, prev_gen_reg);
                }

                for (auto out_reg : out_i) {
                    if (gen_reg == out_reg)
                        continue;
                    addEdge(out_reg, gen_reg);
                }
            }
        }
    }
} // namespace backend
