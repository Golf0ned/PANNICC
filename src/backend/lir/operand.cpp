#include <stdexcept>

#include "backend/lir/operand.h"

namespace backend::lir {
    // TODO: support different calling convention (looking at you, windows)
    // Also applies to callee and caller saved registers
    const std::vector<RegisterNum> &getArgRegisters() {
        static std::vector<RegisterNum> arg_registers = {
            RegisterNum::RDI, RegisterNum::RSI, RegisterNum::RDX,
            RegisterNum::RCX, RegisterNum::R8,  RegisterNum::R9,
        };
        return arg_registers;
    }

    const std::vector<RegisterNum> &getCallerSavedRegisters() {
        static std::vector<RegisterNum> caller_saved = {
            RegisterNum::RAX, RegisterNum::RDI, RegisterNum::RSI,
            RegisterNum::RDX, RegisterNum::RCX, RegisterNum::R8,
            RegisterNum::R9,  RegisterNum::R10, RegisterNum::R11,
        };
        return caller_saved;
    }

    const std::vector<RegisterNum> &getCalleeSavedRegisters() {
        static std::vector<RegisterNum> callee_saved = {
            RegisterNum::RBX, RegisterNum::RSP, RegisterNum::RBP,
            RegisterNum::R12, RegisterNum::R13, RegisterNum::R14,
            RegisterNum::R15,
        };
        return callee_saved;
    }

    Immediate::Immediate(uint64_t value) : value(value) {}

    uint64_t Immediate::getValue() { return value; }

    Register::Register(RegisterNum reg) : reg(reg) {}

    RegisterNum Register::getRegNum() { return reg; }

    VirtualRegister::VirtualRegister(std::string name)
        : Register(RegisterNum::VIRTUAL), name(name) {}

    std::string VirtualRegister::getName() { return name; }

    Address::Address(Register *base, Register *index, Immediate *scale,
                     Immediate *displacement)
        : base(base), index(index), scale(scale), displacement(displacement) {}

    Register *Address::getBase() { return base; }

    Register *Address::getIndex() { return index; }

    Immediate *Address::getScale() { return scale; }

    Immediate *Address::getDisplacement() { return displacement; }

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

    Address *OperandManager::getAddress(Register *base, Register *index,
                                        Immediate *scale,
                                        Immediate *displacement) {
        addresses.push_back(
            std::make_unique<Address>(base, index, scale, displacement));
        return addresses.back().get();
    }
} // namespace backend::lir
