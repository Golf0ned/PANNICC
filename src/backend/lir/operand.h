#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace backend::lir {
    class Operand {
        // toString
    };

    class Immediate : public Operand {
    public:
        Immediate(uint64_t value);
        uint64_t getValue();

    private:
        uint64_t value;
    };

    enum class RegisterNum {
        VIRTUAL,
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

    class Register : public Operand {
    public:
        Register(RegisterNum reg);
        RegisterNum getRegNum();

    private:
        RegisterNum reg;
    };

    class VirtualRegister : public Register {
    public:
        VirtualRegister(std::string name);
        std::string getName();

    private:
        std::string name;
    };

    class OperandManager {
    public:
        Immediate *getImmediate(uint64_t value);
        Register *getRegister(RegisterNum reg);
        VirtualRegister *getRegister(std::string name);

    private:
        std::unordered_map<uint64_t, std::unique_ptr<Immediate>> immediates;
        std::unordered_map<RegisterNum, std::unique_ptr<Register>> registers;
        std::unordered_map<std::string, std::unique_ptr<VirtualRegister>>
            virtual_registers;
    };
} // namespace backend::lir
