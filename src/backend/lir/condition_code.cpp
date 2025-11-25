#include <utility>

#include "backend/lir/condition_code.h"

namespace backend::lir {
    ConditionCode invert(ConditionCode cc) {
        switch (cc) {
        case ConditionCode::EQ:
            return ConditionCode::NEQ;
        case ConditionCode::NEQ:
            return ConditionCode::EQ;
        }
        std::unreachable();
    }

    std::string toString(ConditionCode cc) {
        switch (cc) {
        case ConditionCode::EQ:
            return "e";
        case ConditionCode::NEQ:
            return "ne";
        }
        std::unreachable();
    }
} // namespace backend::lir
