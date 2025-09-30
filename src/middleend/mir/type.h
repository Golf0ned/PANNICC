#pragma once

#include <string>
#include <unordered_map>

namespace middleend::mir {
    enum class Type {
        I1,
        I16,
        I32,
        I64,
        PTR,
    };

    // clang-format off
    const std::unordered_map<std::string, Type> strToType = {
        {"i1", Type::I1},
        {"i16", Type::I16},
        {"i32", Type::I32},
        {"i64", Type::I64},
        {"ptr", Type::PTR},
    };
    // clang-format on

    std::string toString(Type type);
} // namespace middleend::mir
