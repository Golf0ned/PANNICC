#pragma once

#include <cstdint>
#include <string>

namespace backend::lir {
    class Operand {};

    class Immediate : public Operand {
    public:
    private:
        uint64_t value;
    };

    class Register : public Operand {
    public:
    private:
        std::string name;
    };
} // namespace backend::lir
