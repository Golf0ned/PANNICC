#pragma once

#include <string>
#include <unordered_map>

#include "middleend/mir/type.h"

namespace frontend {
    enum class Type { INT };

    const std::unordered_map<std::string, Type> strToType = {
        {"int", Type::INT},
    };

    std::string toString(Type type);

    middleend::mir::Type toMir(Type type);
} // namespace frontend
