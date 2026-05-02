#pragma once

#include "middleend/mir/operator.h"

#include <string>

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

    BinaryOp strToBinaryOp(std::string str);
    std::string toString(BinaryOp op);
    middleend::mir::BinaryOp toMir(BinaryOp op);

    enum class UnaryOp {
        PLUS,
        MINUS,
        NOT,
        DEREF,
        ADDRESS,
    };

    UnaryOp strToUnaryOp(std::string str);
    std::string toString(UnaryOp op);

} // namespace frontend
