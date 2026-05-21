#pragma once

#include <string>

namespace middleend::mir {

enum class BinaryOp {
    ADD,
    SUB,
    MUL,
    SDIV,
    AND,
    OR,
    XOR,
    SHL,
    ASHR,
};

BinaryOp strToBinaryOp(std::string str);
std::string toString(BinaryOp op);

enum class CmpOp {
    SGT,
};

CmpOp strToCmpOp(std::string str);
std::string toString(CmpOp op);

} // namespace middleend::mir
