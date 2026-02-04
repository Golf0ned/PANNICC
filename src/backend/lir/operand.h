#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "backend/lir/register_num.h"

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

    class Register : public Operand {
    public:
        Register(RegisterNum reg);
        RegisterNum getRegNum();
        virtual DataSize getSize();
        virtual std::string toString();

    private:
        RegisterNum reg;
    };

    class VirtualRegister : public Register {
    public:
        VirtualRegister(std::string name, DataSize size);
        std::string getName();
        DataSize getSize() override;
        std::string toString() override;

    private:
        std::string name;
        DataSize size;
    };

    class ConstrainedRegister : public VirtualRegister {
    public:
        ConstrainedRegister(VirtualRegister *reg, RegisterNum constraint);
        RegisterNum getConstraint();
        std::string toString() override;

    private:
        RegisterNum constraint;
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

    class StackArg : public Operand {
    public:
        StackArg(uint64_t arg_num);
        uint64_t getArgNum();
        std::string toString();

    private:
        uint64_t arg_num;
    };

    class OperandManager {
    public:
        uint64_t getNumRegisters();
        Immediate *getImmediate(uint64_t value);
        Register *getRegister(RegisterNum reg);
        VirtualRegister *getRegister(std::string name, DataSize size);
        ConstrainedRegister *getConstrainedRegister(VirtualRegister *reg,
                                                    RegisterNum constraint);
        Address *getAddress(Register *base, Register *index, Immediate *scale,
                            Immediate *displacement);
        StackArg *getStackArg(uint64_t arg_num);

    private:
        std::unordered_map<uint64_t, std::unique_ptr<Immediate>> immediates;
        std::unordered_map<RegisterNum, std::unique_ptr<Register>> registers;
        std::unordered_map<
            std::string,
            std::unordered_map<DataSize, std::unique_ptr<VirtualRegister>>>
            virtual_registers;
        std::vector<std::unique_ptr<ConstrainedRegister>> constrained_registers;
        std::vector<std::unique_ptr<Address>> addresses;
        std::unordered_map<uint64_t, std::unique_ptr<StackArg>> stack_args;
    };
} // namespace backend::lir
