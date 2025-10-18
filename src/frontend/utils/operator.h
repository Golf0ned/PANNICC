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
        OR,
        XOR,
        LSHIFT,
        RSHIFT,
    };

    enum class UnaryOp {
        PLUS,
        MINUS,
        NOT,
    };

    // clang-format off
    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"/", BinaryOp::DIV},
        {"&", BinaryOp::AND},
        {"|", BinaryOp::OR},
        {"^", BinaryOp::XOR},
        {"<<", BinaryOp::LSHIFT},
        {">>", BinaryOp::RSHIFT},
    };
    // clang-format on

    const std::unordered_map<std::string, UnaryOp> strToUnaryOp = {
        {"+", UnaryOp::PLUS},
        {"-", UnaryOp::MINUS},
        {"~", UnaryOp::NOT},
    };

    std::string toString(BinaryOp op);
    std::string toString(UnaryOp op);

    middleend::mir::BinaryOp toMir(BinaryOp op);
} // namespace frontend
