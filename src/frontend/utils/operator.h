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

    enum class UnaryOp {
        PLUS,
        MINUS,
    };

    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"&", BinaryOp::AND},
    };

    const std::unordered_map<std::string, UnaryOp> strToUnaryOp = {
        {"+", UnaryOp::PLUS},
        {"-", UnaryOp::MINUS},
    };

    std::string toString(BinaryOp op);
    std::string toString(UnaryOp op);

    middleend::mir::BinaryOp toMir(BinaryOp op);
} // namespace frontend
