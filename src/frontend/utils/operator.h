#pragma once

#include <string>
#include <unordered_map>

#include "middleend/mir/operator.h"

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

    middleend::mir::BinaryOp toMir(BinaryOp op);
} // namespace frontend
