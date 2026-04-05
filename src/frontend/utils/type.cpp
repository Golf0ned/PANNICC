#include <utility>

#include "frontend/utils/type.h"

namespace frontend {
    std::string toString(Type type) {
        switch (type) {
        case Type::INT:
            return "int";
        case Type::INT_PTR:
            return "int *";
        }
        std::unreachable();
    }

    // TODO: data model support
    middleend::mir::Type toMir(Type type) {
        switch (type) {
        case Type::INT:
            return middleend::mir::Type::I32;
        case Type::INT_PTR:
            return middleend::mir::Type::PTR;
        }
        std::unreachable();
    }
} // namespace frontend
