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
    ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN,
    LSHIFT_ASSIGN,
    RSHIFT_ASSIGN,
};

bool isAssignment(BinaryOp op);
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
