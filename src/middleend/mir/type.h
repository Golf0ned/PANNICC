#pragma once

#include <string>
#include <unordered_map>

namespace middleend {
    enum class Type {
        I64,
        PTR,
    };

    const std::unordered_map<std::string, Type> strToType = {
        {"i64", Type::I64},
        {"ptr", Type::PTR},
    };

    std::string toString(Type type);
} // namespace middleend
