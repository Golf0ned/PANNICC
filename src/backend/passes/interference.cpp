#include <iostream>

#include "backend/lir/operand.h"
#include "backend/passes/interference.h"

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
            auto first_color = lir::toSized(first_num, flat_size);
            auto second_color = lir::toSized(second_num, flat_size);
            return first_color == second_color;
        }

        return false;
    }

    lir::Register *InterferenceBuilder::flattenRegister(lir::Register *reg) {
        auto reg_num = reg->getRegNum();
        if (reg_num == lir::RegisterNum::VIRTUAL) {
            auto virtual_reg = static_cast<lir::VirtualRegister *>(reg);
            return om->getRegister(virtual_reg->getName(), flat_size);
        }

        return om->getRegister(lir::toSized(reg_num, flat_size));
    }

    void InterferenceBuilder::interfere(lir::Register *first,
                                        lir::Register *second) {
        auto flat_first = flattenRegister(first),
             flat_second = flattenRegister(second);

        if (sameReg(flat_first, flat_second))
            return;

        interference[flat_first].insert(flat_second);
        interference[flat_second].insert(flat_first);
    }

    void InterferenceBuilder::addReg(lir::Register *reg) {
        interference[flattenRegister(reg)];
    }

    Interference computeInterference(lir::Function *f, Liveness &l,
                                     lir::OperandManager *om) {
        InterferenceBuilder ib(om);

        auto &physical_regs = lir::getAllRegisters();
        for (size_t r1 = 1; r1 < physical_regs.size(); r1++)
            for (size_t r2 = 0; r2 < r1; r2++)
                ib.interfere(om->getRegister(physical_regs[r1]),
                             om->getRegister(physical_regs[r2]));

        auto gen = l[0], kill = l[1], out = l[3];
        for (size_t i = 0; i < gen.size(); i++) {
            auto &gen_i = gen[i], &kill_i = kill[i], &out_i = out[i];
            for (auto gen_reg : gen_i) {
                for (auto prev_gen_reg : gen_i)
                    ib.interfere(prev_gen_reg, gen_reg);
                for (auto out_reg : out_i)
                    ib.interfere(out_reg, gen_reg);
            }

            for (auto reg : kill_i)
                ib.addReg(reg);
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
