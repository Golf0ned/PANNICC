#include <stdexcept>

#include "backend/lir/operand.h"

namespace backend::lir {
    static uint64_t cur_id = 0;

    Immediate::Immediate(uint64_t value) : value(value) {}

    uint64_t Immediate::getValue() { return value; }

    std::string Immediate::toString() { return "$" + std::to_string(value); }

    Register::Register(RegisterNum reg) : reg(reg), id(cur_id++) {}

    uint64_t Register::getId() { return id; }

    RegisterNum Register::getRegNum() { return reg; }

    DataSize Register::getSize() { return ::backend::lir::getSize(reg); }

    std::string Register::toString() {
        return "%" + ::backend::lir::toString(reg);
    }

    VirtualRegister::VirtualRegister(std::string name, DataSize size)
        : Register(RegisterNum::VIRTUAL), name(name), size(size) {}

    std::string VirtualRegister::getName() { return name; }

    DataSize VirtualRegister::getSize() { return size; }

    std::string VirtualRegister::toString() { return "%" + name; }

    ConstrainedRegister::ConstrainedRegister(VirtualRegister *reg,
                                             RegisterNum constraint)
        : VirtualRegister(reg->getName(), reg->getSize()),
          constraint(constraint) {}

    RegisterNum ConstrainedRegister::getConstraint() { return constraint; }

    std::string ConstrainedRegister::toString() {
        return "[" + VirtualRegister::toString() + " -> %" +
               ::backend::lir::toString(constraint) + "]";
    }

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
            res += std::to_string(displacement->getValue());
        if (!base && !index)
            return res;

        res += '(';

        if (base) {
            res += base->toString();
        }

        if (index) {
            res += ',';
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

    StackArg::StackArg(uint64_t arg_num) : arg_num(arg_num) {}

    uint64_t StackArg::getArgNum() { return arg_num; }

    std::string StackArg::toString() {
        return "(stack " + std::to_string(arg_num) + ")";
    }

    uint64_t OperandManager::getNumRegisters() {
        return 2 * getAllRegisters().size() + virtual_registers.size();
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

    VirtualRegister *OperandManager::getRegister(std::string name,
                                                 DataSize size) {
        if (!virtual_registers.contains(name))
            virtual_registers[name][size] =
                std::make_unique<VirtualRegister>(name, size);

        auto &size_map = virtual_registers.at(name);
        if (!size_map.contains(size))
            size_map[size] = std::make_unique<VirtualRegister>(name, size);

        return size_map.at(size).get();
    }

    ConstrainedRegister *
    OperandManager::getConstrainedRegister(VirtualRegister *reg,
                                           RegisterNum constraint) {
        constrained_registers.push_back(
            std::make_unique<ConstrainedRegister>(reg, constraint));
        return constrained_registers.back().get();
    }

    Address *OperandManager::getAddress(Register *base, Register *index,
                                        Immediate *scale,
                                        Immediate *displacement) {
        addresses.push_back(
            std::make_unique<Address>(base, index, scale, displacement));
        return addresses.back().get();
    }

    StackArg *OperandManager::getStackArg(uint64_t arg_num) {
        if (!stack_args.contains(arg_num))
            stack_args.insert({arg_num, std::make_unique<StackArg>(arg_num)});
        return stack_args.at(arg_num).get();
    }
} // namespace backend::lir
