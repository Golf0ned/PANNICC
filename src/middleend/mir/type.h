#pragma once

#include <string>

namespace middleend::mir {

enum class Type {
    I1,
    I16,
    I32,
    I64,
    PTR,
};

Type strToType(std::string str);
std::string toString(Type type);

} // namespace middleend::mir
