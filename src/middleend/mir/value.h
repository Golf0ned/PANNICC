#pragma once

#include "middleend/mir/type.h"

namespace middleend::mir {
    // TODO: function params
    class Value {
    public:
        Value(Type type);
        Type getType();
        virtual ~Value() = default;

    private:
        Type type;
    };

    class Literal : public Value {
    public:
        Literal(Type type, int64_t value);
        int64_t getValue();

    private:
        int64_t value;
    };
} // namespace middleend::mir
