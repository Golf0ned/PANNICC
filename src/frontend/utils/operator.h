#pragma once

#include <string>
#include <unordered_map>

namespace frontend {
    enum class BinaryOp {
        ADD,
        SUB,
        MUL,
        AND,
    };

    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"&", BinaryOp::AND},
    };

    std::string toString(BinaryOp op);
} // namespace frontend
