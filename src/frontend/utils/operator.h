#pragma once

#include <string>
#include <unordered_map>

#include "middleend/mir/operator.h"

namespace frontend {
    enum class BinaryOp {
        ADD,
        SUB,
        MUL,
        DIV,
        AND,
    };

    enum class UnaryOp {
        PLUS,
        MINUS,
    };

    // clang-format off
    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"/", BinaryOp::DIV},
        {"&", BinaryOp::AND},
    };
    // clang-format on

    const std::unordered_map<std::string, UnaryOp> strToUnaryOp = {
        {"+", UnaryOp::PLUS},
        {"-", UnaryOp::MINUS},
    };

    std::string toString(BinaryOp op);
    std::string toString(UnaryOp op);

    middleend::mir::BinaryOp toMir(BinaryOp op);
} // namespace frontend
