#include <utility>

#include "frontend/utils/type.h"

namespace frontend {
    std::string toString(Type type) {
        switch (type) {
        case Type::SHORT:
            return "short";
        case Type::INT:
            return "int";
        case Type::LONG:
            return "long";
        case Type::LONG_LONG:
            return "long long";
        }
        std::unreachable();
    }

    middleend::mir::Type toMir(Type type) {
        switch (type) {
        case Type::SHORT:
        case Type::INT:
        case Type::LONG:
        case Type::LONG_LONG:
            return middleend::mir::Type::I64;
        }
        std::unreachable();
    }
} // namespace frontend
