#pragma once

#include <string>
#include <unordered_map>


namespace frontend {
    enum class Type {
        INT64
    };

    const std::unordered_map<std::string, Type> strToType = {
        {"int64_t", Type::INT64},
    };

    std::string toString(Type type);
}
