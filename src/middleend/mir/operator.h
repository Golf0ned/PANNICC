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
        OR,
        XOR,
        SHL,
        ASHR,
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
        {"or", BinaryOp::OR},
        {"xor", BinaryOp::XOR},
        {"shl", BinaryOp::SHL},
        {"ashr", BinaryOp::ASHR},
    };
    // clang-format on

    const std::unordered_map<std::string, CmpOp> strToCmpOp = {
        {"sgt", CmpOp::SGT},
    };

    std::string toString(BinaryOp op);

    std::string toString(CmpOp op);
} // namespace middleend::mir
