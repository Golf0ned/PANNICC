#include <stdexcept>
#include <utility>

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

    std::string Immediate::toString() { return "$" + std::to_string(value); }

    std::string toString(RegisterNum rn) {
        switch (rn) {
        case RegisterNum::VIRTUAL:
            return "virtual";
        case RegisterNum::RAX:
            return "rax";
        case RegisterNum::RCX:
            return "rcx";
        case RegisterNum::RDX:
            return "rdx";
        case RegisterNum::RBX:
            return "rbx";
        case RegisterNum::RSI:
            return "rsi";
        case RegisterNum::RDI:
            return "rdi";
        case RegisterNum::RSP:
            return "rsp";
        case RegisterNum::RBP:
            return "rbp";
        case RegisterNum::R8:
            return "r8";
        case RegisterNum::R9:
            return "r9";
        case RegisterNum::R10:
            return "r10";
        case RegisterNum::R11:
            return "r11";
        case RegisterNum::R12:
            return "r12";
        case RegisterNum::R13:
            return "r13";
        case RegisterNum::R14:
            return "r14";
        case RegisterNum::R15:
            return "r15";
        case RegisterNum::EAX:
            return "eax";
        case RegisterNum::ECX:
            return "ecx";
        case RegisterNum::EDX:
            return "edx";
        case RegisterNum::EBX:
            return "ebx";
        case RegisterNum::ESI:
            return "esi";
        case RegisterNum::EDI:
            return "edi";
        case RegisterNum::ESP:
            return "esp";
        case RegisterNum::EBP:
            return "ebp";
        case RegisterNum::R8D:
            return "r8d";
        case RegisterNum::R9D:
            return "r9d";
        case RegisterNum::R10D:
            return "r10d";
        case RegisterNum::R11D:
            return "r11d";
        case RegisterNum::R12D:
            return "r12d";
        case RegisterNum::R13D:
            return "r13d";
        case RegisterNum::R14D:
            return "r14d";
        case RegisterNum::R15D:
            return "r15d";
        }
        std::unreachable();
    }

    Register::Register(RegisterNum reg) : reg(reg) {}

    std::string Register::toString() {
        return "%" + ::backend::lir::toString(reg);
    }

    RegisterNum Register::getRegNum() { return reg; }

    VirtualRegister::VirtualRegister(std::string name)
        : Register(RegisterNum::VIRTUAL), name(name) {}

    std::string VirtualRegister::getName() { return name; }

    std::string VirtualRegister::toString() { return "%" + name; }

    Address::Address(Register *base, Register *index, Immediate *scale,
                     Immediate *displacement)
        : base(base), index(index), scale(scale), displacement(displacement) {}

    Register *Address::getBase() { return base; }

    Register *Address::getIndex() { return index; }

    Immediate *Address::getScale() { return scale; }

    Immediate *Address::getDisplacement() { return displacement; }

    std::string Address::toString() {
        std::string res = "";

        if (displacement->getValue())
            res += displacement->toString();
        if (!base && !index)
            return res;

        res += '(';

        if (base) {
            res += base->toString();
            if (index)
                res += ',';
        }

        if (index) {
            res += index->toString();
            auto scale_value = scale->getValue();
            if (scale_value > 1) {
                res += ',';
                res += std::to_string(scale_value);
            }
        }

        res += ')';
        return res;
    }

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
