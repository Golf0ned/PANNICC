#include "frontend/utils/type.h"


namespace frontend {
    std::string toString(Type type) {
        switch (type) {
            case Type::INT64: return "int64_t";
            default:          return "UNKNOWN";
        }
    }
}
