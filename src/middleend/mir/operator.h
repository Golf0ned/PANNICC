#pragma once

#include <string>
#include <unordered_map>

namespace middleend::mir {
    enum class BinaryOp {
        ADD,
        SUB,
        MUL,
        AND,
    };

    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"add", BinaryOp::ADD},
        {"sub", BinaryOp::SUB},
        {"mul", BinaryOp::MUL},
        {"and", BinaryOp::AND},
    };

    std::string toString(BinaryOp op);
} // namespace middleend::mir
