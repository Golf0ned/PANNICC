#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace backend::lir {
    class Operand {
    public:
        virtual std::string toString() = 0;
    };

    class Immediate : public Operand {
    public:
        Immediate(uint64_t value);
        uint64_t getValue();
        std::string toString();

    private:
        uint64_t value;
    };

    enum class RegisterNum {
        VIRTUAL,
        RAX,
        RCX,
        RDX,
        RBX,
        RSI,
        RDI,
        RSP,
        RBP,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        EAX,
        ECX,
        EDX,
        EBX,
        ESI,
        EDI,
        ESP,
        EBP,
        R8D,
        R9D,
        R10D,
        R11D,
        R12D,
        R13D,
        R14D,
        R15D,
    };

    std::string toString(RegisterNum rn);

    const std::vector<RegisterNum> &getArgRegisters();
    const std::vector<RegisterNum> &getCalleeSavedRegisters();
    const std::vector<RegisterNum> &getCallerSavedRegisters();

    class Register : public Operand {
    public:
        Register(RegisterNum reg);
        RegisterNum getRegNum();
        virtual std::string toString();

    private:
        RegisterNum reg;
    };

    class VirtualRegister : public Register {
    public:
        VirtualRegister(std::string name);
        std::string getName();
        std::string toString();

    private:
        std::string name;
    };

    class Address : public Operand {
    public:
        Address(Register *base, Register *index, Immediate *scale,
                Immediate *displacement);
        Register *getBase();
        Register *getIndex();
        Immediate *getScale();
        Immediate *getDisplacement();
        std::string toString();

    private:
        Register *base;
        Register *index;
        Immediate *scale;
        Immediate *displacement;
    };

    class OperandManager {
    public:
        Immediate *getImmediate(uint64_t value);
        Register *getRegister(RegisterNum reg);
        VirtualRegister *getRegister(std::string name);
        Address *getAddress(Register *base, Register *index, Immediate *scale,
                            Immediate *displacement);

    private:
        std::unordered_map<uint64_t, std::unique_ptr<Immediate>> immediates;
        std::unordered_map<RegisterNum, std::unique_ptr<Register>> registers;
        std::unordered_map<std::string, std::unique_ptr<VirtualRegister>>
            virtual_registers;
        std::vector<std::unique_ptr<Address>> addresses;
    };
} // namespace backend::lir
