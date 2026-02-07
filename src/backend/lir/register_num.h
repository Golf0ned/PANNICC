#pragma once

#include <vector>

#include "backend/lir/data_size.h"

namespace backend::lir {
    enum class RegisterNum {
        VIRTUAL,

        // 64-bit
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

        // 32-bit
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

    DataSize getSize(RegisterNum rn);

    RegisterNum toSized(RegisterNum rn, DataSize size);

    std::string toString(RegisterNum rn);

    const std::vector<RegisterNum> &getAllRegisters();
    const std::vector<RegisterNum> &getArgRegisters();
    const std::vector<RegisterNum> &getCalleeSavedRegisters();
    const std::vector<RegisterNum> &getCallerSavedRegisters();
    const std::vector<RegisterNum> &getColoringPriority();
} // namespace backend::lir
