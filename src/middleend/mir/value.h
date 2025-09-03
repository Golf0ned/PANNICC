#pragma once

#include "middleend/mir/type.h"

namespace middleend::mir {
    // TODO: function params
    class Value {
    public:
        Type getType();

    private:
        Type type;
    };

    class Literal : public Value {
    public:
        int64_t getValue();

    private:
        int64_t value;
    };
} // namespace middleend::mir
