#include <utility>

#include "middleend/mir/type.h"

namespace middleend::mir {
    std::string toString(Type type) {
        switch (type) {
        case Type::I1:
            return "i1";
        case Type::I64:
            return "i64";
        case Type::PTR:
            return "ptr";
        }
        std::unreachable();
    }
} // namespace middleend::mir
