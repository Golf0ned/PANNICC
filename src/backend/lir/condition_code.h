#pragma once

#include <string>

namespace backend::lir {
    enum class ConditionCode {
        EQ,
        NEQ,
    };

    ConditionCode invert(ConditionCode cc);

    std::string toString(ConditionCode cc);
} // namespace backend::lir
