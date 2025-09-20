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

    enum class CmpOp {
        SGT,
    };

    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"add", BinaryOp::ADD},
        {"sub", BinaryOp::SUB},
        {"mul", BinaryOp::MUL},
        {"and", BinaryOp::AND},
    };

    const std::unordered_map<std::string, CmpOp> strToCmpOp = {
        {"sgt", CmpOp::SGT},
    };

    std::string toString(BinaryOp op);

    std::string toString(CmpOp op);
} // namespace middleend::mir
