#include "middleend/mir/type.h"

namespace middleend::mir {
    std::string toString(Type type) {
        switch (type) {
        case Type::I64:
            return "i64";
        case Type::PTR:
            return "ptr";
        default:
            return "UNKNOWN";
        }
    }
} // namespace middleend::mir
