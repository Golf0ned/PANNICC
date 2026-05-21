#include "middleend/mir/type.h"

#include <unordered_map>
#include <utility>

namespace middleend::mir {

Type strToType(std::string str) {
    static const std::unordered_map<std::string, Type> fromStr = {
        // clang-format off
        {"i1", Type::I1},
        {"i16", Type::I16},
        {"i32", Type::I32},
        {"i64", Type::I64},
        {"ptr", Type::PTR},
        // clang-format on
    };

    return fromStr.at(str);
}

std::string toString(Type type) {
    switch (type) {
    case Type::I1:
        return "i1";
    case Type::I16:
        return "i16";
    case Type::I32:
        return "i32";
    case Type::I64:
        return "i64";
    case Type::PTR:
        return "ptr";
    }
    std::unreachable();
}

} // namespace middleend::mir
