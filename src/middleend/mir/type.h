#pragma once

#include <string>
#include <unordered_map>

namespace middleend {
    enum class Type {
        I64,
    };

    const std::unordered_map<std::string, Type> strToType = {
        {"i64", Type::I64},
    };

    std::string toString(Type type);
} // namespace middleend
