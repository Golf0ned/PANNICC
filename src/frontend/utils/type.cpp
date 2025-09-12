#include <utility>

#include "frontend/utils/type.h"

namespace frontend {
    std::string toString(Type type) {
        switch (type) {
        case Type::INT64:
            return "int64_t";
        }
        std::unreachable();
    }

    middleend::mir::Type toMIR(Type type) {
        switch (type) {
        case Type::INT64:
            return middleend::mir::Type::I64;
        }
        std::unreachable();
    }
} // namespace frontend
