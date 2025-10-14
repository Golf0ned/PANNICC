#pragma once

#include <string>
#include <unordered_map>

namespace middleend::mir {
    enum class BinaryOp {
        ADD,
        SUB,
        MUL,
        SDIV,
        AND,
    };

    enum class CmpOp {
        SGT,
    };

    // clang-format off
    const std::unordered_map<std::string, BinaryOp> strToBinaryOp = {
        {"add", BinaryOp::ADD},
        {"sub", BinaryOp::SUB},
        {"mul", BinaryOp::MUL},
        {"sdiv", BinaryOp::SDIV},
        {"and", BinaryOp::AND},
    };
    // clang-format on

    const std::unordered_map<std::string, CmpOp> strToCmpOp = {
        {"sgt", CmpOp::SGT},
    };

    std::string toString(BinaryOp op);

    std::string toString(CmpOp op);
} // namespace middleend::mir
