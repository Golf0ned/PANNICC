#include <utility>

#include "middleend/mir/operator.h"

namespace middleend::mir {
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

    std::string toString(CmpOp op) {
        switch (op) {
        case CmpOp::SGT:
            return "sgt";
        }
        std::unreachable();
    }
} // namespace middleend::mir
