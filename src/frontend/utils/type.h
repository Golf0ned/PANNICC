#pragma once

#include <string>
#include <unordered_map>

#include "middleend/mir/type.h"

namespace frontend {
    enum class Type { SHORT, INT, LONG, LONG_LONG, FUNCTION };

    const std::unordered_map<std::string, Type> strToType = {
        {"short", Type::SHORT},
        {"int", Type::INT},
        {"long", Type::LONG},
        {"long long", Type::LONG_LONG},
    };

    std::string toString(Type type);

    middleend::mir::Type toMir(Type type);
} // namespace frontend
