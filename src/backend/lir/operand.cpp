#include <stdexcept>

#include "backend/lir/operand.h"

namespace backend::lir {
    Immediate::Immediate(uint64_t value) : value(value) {}

    uint64_t Immediate::getValue() { return value; }

    Register::Register(RegisterNum reg) : reg(reg) {}

    RegisterNum Register::getRegNum() { return reg; }

    VirtualRegister::VirtualRegister(std::string name)
        : Register(RegisterNum::VIRTUAL), name(name) {}

    std::string VirtualRegister::getName() { return name; }

    Immediate *OperandManager::getImmediate(uint64_t value) {
        if (!immediates.contains(value))
            immediates.insert({value, std::make_unique<Immediate>(value)});
        return immediates.at(value).get();
    }

    Register *OperandManager::getRegister(RegisterNum reg) {
        if (reg == RegisterNum::VIRTUAL)
            throw std::invalid_argument("reg must not be virtual");

        if (!registers.contains(reg))
            registers.insert({reg, std::make_unique<Register>(reg)});
        return registers.at(reg).get();
    }

    VirtualRegister *OperandManager::getRegister(std::string name) {
        if (!virtual_registers.contains(name))
            virtual_registers.insert(
                {name, std::make_unique<VirtualRegister>(name)});
        return virtual_registers.at(name).get();
    }
} // namespace backend::lir
