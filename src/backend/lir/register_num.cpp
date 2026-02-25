#include <utility>

#include "backend/lir/register_num.h"

namespace backend::lir {
    DataSize getSize(RegisterNum rn) {
        switch (rn) {
        case RegisterNum::RAX:
        case RegisterNum::RCX:
        case RegisterNum::RDX:
        case RegisterNum::RBX:
        case RegisterNum::RSI:
        case RegisterNum::RDI:
        case RegisterNum::RSP:
        case RegisterNum::RBP:
        case RegisterNum::R8:
        case RegisterNum::R9:
        case RegisterNum::R10:
        case RegisterNum::R11:
        case RegisterNum::R12:
        case RegisterNum::R13:
        case RegisterNum::R14:
        case RegisterNum::R15:
            return DataSize::QUADWORD;
        case RegisterNum::EAX:
        case RegisterNum::ECX:
        case RegisterNum::EDX:
        case RegisterNum::EBX:
        case RegisterNum::ESI:
        case RegisterNum::EDI:
        case RegisterNum::ESP:
        case RegisterNum::EBP:
        case RegisterNum::R8D:
        case RegisterNum::R9D:
        case RegisterNum::R10D:
        case RegisterNum::R11D:
        case RegisterNum::R12D:
        case RegisterNum::R13D:
        case RegisterNum::R14D:
        case RegisterNum::R15D:
            return DataSize::DOUBLEWORD;
        case RegisterNum::VIRTUAL:
            std::unreachable();
        }
        std::unreachable();
    }

    // TODO: implement for other register sizes. or dont. its up to you! :)
    RegisterNum toSized(RegisterNum rn, DataSize size) {
        switch (rn) {
        case RegisterNum::VIRTUAL:
            return RegisterNum::VIRTUAL;
        case RegisterNum::RAX:
        case RegisterNum::EAX:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::EAX;
            case DataSize::QUADWORD:
                return RegisterNum::RAX;
            }
        case RegisterNum::RCX:
        case RegisterNum::ECX:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::ECX;
            case DataSize::QUADWORD:
                return RegisterNum::RCX;
            }
        case RegisterNum::RDX:
        case RegisterNum::EDX:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::EDX;
            case DataSize::QUADWORD:
                return RegisterNum::RDX;
            }
        case RegisterNum::RBX:
        case RegisterNum::EBX:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::EBX;
            case DataSize::QUADWORD:
                return RegisterNum::RBX;
            }
        case RegisterNum::RSI:
        case RegisterNum::ESI:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::ESI;
            case DataSize::QUADWORD:
                return RegisterNum::RSI;
            }
        case RegisterNum::RDI:
        case RegisterNum::EDI:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::EDI;
            case DataSize::QUADWORD:
                return RegisterNum::RDI;
            }
        case RegisterNum::RSP:
        case RegisterNum::ESP:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::ESP;
            case DataSize::QUADWORD:
                return RegisterNum::RSP;
            }
        case RegisterNum::RBP:
        case RegisterNum::EBP:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::EBP;
            case DataSize::QUADWORD:
                return RegisterNum::RBP;
            }
        case RegisterNum::R8:
        case RegisterNum::R8D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R8D;
            case DataSize::QUADWORD:
                return RegisterNum::R8;
            }
        case RegisterNum::R9:
        case RegisterNum::R9D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R9D;
            case DataSize::QUADWORD:
                return RegisterNum::R9;
            }
        case RegisterNum::R10:
        case RegisterNum::R10D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R10D;
            case DataSize::QUADWORD:
                return RegisterNum::R10;
            }
        case RegisterNum::R11:
        case RegisterNum::R11D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R11D;
            case DataSize::QUADWORD:
                return RegisterNum::R11;
            }
        case RegisterNum::R12:
        case RegisterNum::R12D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R12D;
            case DataSize::QUADWORD:
                return RegisterNum::R12;
            }
        case RegisterNum::R13:
        case RegisterNum::R13D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R13D;
            case DataSize::QUADWORD:
                return RegisterNum::R13;
            }
        case RegisterNum::R14:
        case RegisterNum::R14D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R14D;
            case DataSize::QUADWORD:
                return RegisterNum::R14;
            }
        case RegisterNum::R15:
        case RegisterNum::R15D:
            switch (size) {
            default:
                std::unreachable();
            case DataSize::DOUBLEWORD:
                return RegisterNum::R15D;
            case DataSize::QUADWORD:
                return RegisterNum::R15;
            }
        }
        std::unreachable();
    }

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

    // TODO: support different calling convention (looking at you, windows)
    const std::vector<RegisterNum> &getAllRegisters() {
        static std::vector<RegisterNum> all_registers = {
            RegisterNum::RAX, RegisterNum::RCX, RegisterNum::RDX,
            RegisterNum::RBX, RegisterNum::RSI, RegisterNum::RDI,
            RegisterNum::RSP, RegisterNum::RBP, RegisterNum::R8,
            RegisterNum::R9,  RegisterNum::R10, RegisterNum::R11,
            RegisterNum::R12, RegisterNum::R13, RegisterNum::R14,
            RegisterNum::R15,
        };
        return all_registers;
    }

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

    const std::vector<RegisterNum> &getColoringPriority() {
        static std::vector<RegisterNum> coloring_priority = {
            RegisterNum::R10, RegisterNum::R11, RegisterNum::RDI,
            RegisterNum::RSI, RegisterNum::RDX, RegisterNum::RCX,
            RegisterNum::R8,  RegisterNum::R9,  RegisterNum::RAX,
            RegisterNum::RBX, RegisterNum::R12, RegisterNum::R13,
            RegisterNum::R14, RegisterNum::R15, RegisterNum::RBP,
        };
        return coloring_priority;
    }
} // namespace backend::lir
