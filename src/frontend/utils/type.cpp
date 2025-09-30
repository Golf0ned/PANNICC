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
        case Type::FUNCTION:
            std::unreachable();
        }
        std::unreachable();
    }

    // TODO: data model support
    middleend::mir::Type toMir(Type type) {
        switch (type) {
        case Type::SHORT:
            return middleend::mir::Type::I16;
        case Type::INT:
            return middleend::mir::Type::I32;
        case Type::LONG:
            return middleend::mir::Type::I64;
        case Type::LONG_LONG:
            return middleend::mir::Type::I64;
        case Type::FUNCTION:
            std::unreachable();
        }
        std::unreachable();
    }
} // namespace frontend
