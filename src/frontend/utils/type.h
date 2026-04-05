#pragma once

#include <string>

#include "middleend/mir/type.h"

namespace frontend {
    enum class Type { INT, INT_PTR };

    std::string toString(Type type);

    middleend::mir::Type toMir(Type type);
} // namespace frontend
