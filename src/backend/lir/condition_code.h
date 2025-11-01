#pragma once

namespace backend::lir {
    enum class ConditionCode {
        EQ,
        NEQ,
    };

    ConditionCode invert(ConditionCode cc);
} // namespace backend::lir
