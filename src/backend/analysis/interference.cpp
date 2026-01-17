#include <iostream>

#include "backend/analysis/interference.h"
#include "backend/lir/operand.h"

namespace backend {
    Interference::Interference(lir::Program &p) : om(p.getOm()) {}

    void Interference::addRegister(lir::Register *reg) {
        if (!reg_to_index.contains(reg))
            reg_to_index[reg] = reg_to_index.size();
    }

    void Interference::addEdge(lir::Register *first, lir::Register *second) {
        auto first_idx = reg_to_index.at(first),
             second_idx = reg_to_index.at(second);

        adj_matrix[first_idx][second_idx] = true;
        adj_matrix[second_idx][first_idx] = true;
    }

    bool Interference::sameReg(lir::Register *first, lir::Register *second) {
        auto first_num = first->getRegNum(), second_num = second->getRegNum();

        if (first_num == lir::RegisterNum::VIRTUAL &&
            second_num == lir::RegisterNum::VIRTUAL) {
            auto first_virtual = static_cast<lir::VirtualRegister *>(first);
            auto second_virtual = static_cast<lir::VirtualRegister *>(second);
            return first_virtual->getName() == second_virtual->getName();
        }

        if (first_num != lir::RegisterNum::VIRTUAL &&
            second_num != lir::RegisterNum::VIRTUAL) {
            auto first_64 = lir::toSized(first_num, lir::DataSize::QUADWORD);
            auto second_64 = lir::toSized(second_num, lir::DataSize::QUADWORD);
            return first_64 == second_64;
        }

        return false;
    }

    std::vector<lir::Register *>
    Interference::getSizedRegisters(lir::Register *reg) {
        auto reg_num = reg->getRegNum();
        if (reg_num == lir::RegisterNum::VIRTUAL) {
            return {reg};
        }

        return {
            om->getRegister(lir::toSized(reg_num, lir::DataSize::QUADWORD)),
            om->getRegister(lir::toSized(reg_num, lir::DataSize::DOUBLEWORD)),
        };
    }

    void Interference::addAllEdges(lir::Register *first,
                                   lir::Register *second) {
        auto first_regs = getSizedRegisters(first),
             second_regs = getSizedRegisters(second);

        for (auto first_reg : first_regs)
            for (auto second_reg : second_regs)
                addEdge(first_reg, second_reg);
    }

    void Interference::addPhysicalRegisters() {
        auto &all_regs = lir::getAllRegisters();

        for (auto reg : all_regs) {
            auto new_reg_64 = om->getRegister(reg);
            auto new_reg_32 =
                om->getRegister(lir::toSized(reg, lir::DataSize::DOUBLEWORD));

            addRegister(new_reg_64);
            addRegister(new_reg_32);
        }

        for (auto reg1 : all_regs) {
            for (auto reg2 : all_regs) {
                if (reg1 == reg2)
                    break;
                addAllEdges(om->getRegister(reg1), om->getRegister(reg2));
            }
        }
    }

    void Interference::computeInterference(Liveness &liveness) {
        auto num_regs = om->getNumRegisters();
        adj_matrix = std::vector<std::vector<bool>>(
            num_regs, std::vector<bool>(num_regs, false));
        reg_to_index.clear();

        addPhysicalRegisters();

        auto gen = liveness[0], out = liveness[3];
        for (size_t f = 0; f < out.size(); f++) {
            for (size_t i = 0; i < out[f].size(); i++) {
                auto &gen_i = gen[f][i], &out_i = out[f][i];

                for (auto gen_reg : gen_i)
                    addRegister(gen_reg);
                for (auto out_reg : out_i)
                    addRegister(out_reg);

                for (auto gen_reg : gen_i) {
                    for (auto prev_gen_reg : gen_i) {
                        if (sameReg(prev_gen_reg, gen_reg))
                            continue;
                        addAllEdges(prev_gen_reg, gen_reg);
                    }

                    for (auto out_reg : out_i) {
                        if (sameReg(out_reg, gen_reg))
                            continue;
                        addAllEdges(out_reg, gen_reg);
                    }
                }
            }
        }
    }

    void Interference::printInterference() {
        std::vector<lir::Register *> index_to_reg(reg_to_index.size());
        for (auto &[reg, idx] : reg_to_index)
            index_to_reg[idx] = reg;

        size_t r = adj_matrix.size(), c = adj_matrix[0].size();
        for (size_t i = 0; i < r; i++) {
            std::cout << index_to_reg[i]->toString() << ": {";

            bool first_element = true;
            for (size_t j = 0; j < c; j++) {
                if (adj_matrix[i][j]) {
                    if (first_element)
                        first_element = false;
                    else
                        std::cout << ", ";
                    std::cout << index_to_reg[j]->toString();
                }
            }

            std::cout << "}" << std::endl;
        }
    }
} // namespace backend
