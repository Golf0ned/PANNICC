#include "middleend/mir/type.h"

namespace middleend {
    std::string toString(Type type) {
        switch (type) {
            case Type::I64:
                return "i64";
            default:
                return "UNKNOWN";
        }
    }
} // namespace middleend
