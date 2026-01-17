#include <iostream>

#include "backend/analysis/interference.h"
#include "backend/lir/operand.h"

namespace backend {
    InterferenceBuilder::InterferenceBuilder(lir::OperandManager *om)
        : om(om) {}

    Interference InterferenceBuilder::getInterference() { return interference; }

    bool InterferenceBuilder::sameReg(lir::Register *first,
                                      lir::Register *second) {
        auto first_num = first->getRegNum();
        auto first_is_virtual = first_num == lir::RegisterNum::VIRTUAL;

        auto second_num = second->getRegNum();
        auto second_is_virtual = second_num == lir::RegisterNum::VIRTUAL;

        if (first_is_virtual && second_is_virtual) {
            auto first_virtual = static_cast<lir::VirtualRegister *>(first);
            auto second_virtual = static_cast<lir::VirtualRegister *>(second);
            return first_virtual->getName() == second_virtual->getName();
        }

        if (!first_is_virtual && !second_is_virtual) {
            auto size = lir::DataSize::QUADWORD;
            auto first_64 = lir::toSized(first_num, size);
            auto second_64 = lir::toSized(second_num, size);
            return first_64 == second_64;
        }

        return false;
    }

    std::vector<lir::Register *>
    InterferenceBuilder::getSizedRegisters(lir::Register *reg) {
        auto reg_num = reg->getRegNum();
        if (reg_num == lir::RegisterNum::VIRTUAL)
            return {reg};

        return {
            om->getRegister(lir::toSized(reg_num, lir::DataSize::QUADWORD)),
            om->getRegister(lir::toSized(reg_num, lir::DataSize::DOUBLEWORD)),
        };
    }

    void InterferenceBuilder::interfere(lir::Register *first,
                                        lir::Register *second) {
        if (sameReg(first, second))
            return;

        auto first_regs = getSizedRegisters(first),
             second_regs = getSizedRegisters(second);

        for (auto first_reg : first_regs)
            for (auto second_reg : second_regs) {
                interference[first].insert(second);
                interference[second].insert(first);
            }
    }

    Interference computeInterference(lir::Program &p, Liveness &l) {
        auto om = p.getOm();
        InterferenceBuilder ib(om);

        auto &physical_regs = lir::getAllRegisters();
        for (size_t r1 = 1; r1 < physical_regs.size(); r1++)
            for (size_t r2 = 0; r2 < r1; r2++)
                ib.interfere(om->getRegister(physical_regs[r1]),
                             om->getRegister(physical_regs[r2]));

        auto gen = l[0], out = l[3];
        for (size_t f = 0; f < gen.size(); f++) {
            for (size_t i = 0; i < gen[f].size(); i++) {
                auto &gen_i = gen[f][i], &out_i = out[f][i];
                for (auto gen_reg : gen_i) {
                    for (auto prev_gen_reg : gen_i)
                        ib.interfere(prev_gen_reg, gen_reg);
                    for (auto out_reg : out_i)
                        ib.interfere(out_reg, gen_reg);
                }
            }
        }

        return ib.getInterference();
    }

    void printInterference(Interference &i) {
        // TODO: sort?
        for (auto &[r1, r2_set] : i) {
            std::cout << r1->toString() << ": {";
            for (auto iter = r2_set.begin(); iter != r2_set.end(); iter++) {
                if (iter != r2_set.begin())
                    std::cout << ", ";
                std::cout << (*iter)->toString();
            }
            std::cout << "}" << std::endl;
        }
    }
} // namespace backend
