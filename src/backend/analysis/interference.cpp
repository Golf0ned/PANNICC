#include <iostream>

#include "backend/analysis/interference.h"
#include "backend/lir/operand.h"

namespace backend {
    Interference::Interference(lir::Program &p) : program(p) {}

    void Interference::addRegister(lir::Register *reg) {
        if (!reg_to_index.contains(reg))
            reg_to_index[reg] = reg_to_index.size();
    }

    void Interference::addEdge(lir::Register *first, lir::Register *second) {
        addRegister(first);
        addRegister(second);

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
            addRegister(new_reg);
            for (auto &[prev_reg, _] : reg_to_index) {
                if (prev_reg == new_reg)
                    continue;
                addEdge(prev_reg, new_reg);
            }
        }

        auto out = liveness.getOut();
        auto gen = liveness.getGen();

        for (size_t i = 0; i < out.size(); i++) {
            auto &gen_i = gen[i], &out_i = out[i];
            for (auto gen_reg : gen_i) {
                // TODO: make this not ugly thanks
                for (auto prev_gen_reg : gen_i) {
                    if (gen_reg == prev_gen_reg)
                        continue;
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

    void Interference::printInterference() {
        for (auto &[reg, idx] : reg_to_index) {
            std::cout << reg->toString() << ": " << idx << std::endl;
        }

        std::cout << std::endl;

        size_t r = adj_matrix.size(), c = adj_matrix[0].size();
        for (size_t i = 0; i < r; i++) {
            std::cout << i << ": {";
            for (size_t j = 0; j < c; j++) {
                if (j)
                    std::cout << ", ";
                std::cout << adj_matrix[i][j];
            }
            std::cout << "}" << std::endl;
        }
    }
} // namespace backend
