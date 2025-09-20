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

    enum class CmpOp {
        GT,
    };

    const std::unordered_map<std::string, CmpOp> strToCmpOp = {
        {">", CmpOp::GT},
    };

    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"&", BinaryOp::AND},
    };

    std::string toString(BinaryOp op);

    std::string toString(CmpOp op);

    middleend::mir::BinaryOp toMir(BinaryOp op);

    middleend::mir::CmpOp toMir(CmpOp op, bool isSigned);
} // namespace frontend
