#pragma once

#include <cstdint>
#include <vector>

#include "middleend/mir/type.h"

namespace middleend::mir {
    class Instruction;

    // TODO: function params
    class Value {
    public:
        Value(Type type);
        Type getType();
        std::unordered_map<Instruction *, uint64_t> &getUses();
        virtual ~Value() = default;

    private:
        Type type;
        std::unordered_map<Instruction *, uint64_t> uses;
    };

    class Literal : public Value {
    public:
        Literal(Type type, int64_t value);
        int64_t getValue();

    private:
        int64_t value;
    };
} // namespace middleend::mir
