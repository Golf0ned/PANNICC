#include "middleend/mir/operator.h"

#include <unordered_map>
#include <utility>

namespace middleend::mir {

BinaryOp strToBinaryOp(std::string str) {
    static const std::unordered_map<std::string, BinaryOp> fromStr = {
        // clang-format off
        {"add", BinaryOp::ADD},
        {"sub", BinaryOp::SUB},
        {"mul", BinaryOp::MUL},
        {"sdiv", BinaryOp::SDIV},
        {"and", BinaryOp::AND},
        {"or", BinaryOp::OR},
        {"xor", BinaryOp::XOR},
        {"shl", BinaryOp::SHL},
        {"ashr", BinaryOp::ASHR},
        // clang-format on
    };

    return fromStr.at(str);
}

std::string toString(BinaryOp op) {
    switch (op) {
    case BinaryOp::ADD:
        return "add";
    case BinaryOp::SUB:
        return "sub";
    case BinaryOp::MUL:
        return "mul";
    case BinaryOp::SDIV:
        return "sdiv";
    case BinaryOp::AND:
        return "and";
    case BinaryOp::OR:
        return "or";
    case BinaryOp::XOR:
        return "xor";
    case BinaryOp::SHL:
        return "shl";
    case BinaryOp::ASHR:
        return "ashr";
    }
    std::unreachable();
}

CmpOp strToCmpOp(std::string str) {
    static const std::unordered_map<std::string, CmpOp> fromStr = {
        // clang-format off
        {"sgt", CmpOp::SGT},
        // clang-format on
    };

    return fromStr.at(str);
}

std::string toString(CmpOp op) {
    switch (op) {
    case CmpOp::SGT:
        return "sgt";
    }
    std::unreachable();
}

} // namespace middleend::mir
