#pragma once

#include <string>
#include <unordered_map>

namespace middleend::mir {
    enum class Type {
        I1,
        I64,
        PTR,
    };

    const std::unordered_map<std::string, Type> strToType = {
        {"i1", Type::I1},
        {"i64", Type::I64},
        {"ptr", Type::PTR},
    };

    std::string toString(Type type);
} // namespace middleend::mir
