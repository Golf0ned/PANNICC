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
        case BinaryOp::AND:
            return "and";
        default:
            return "UNKNOWN";
        }
    }

    middleend::mir::BinaryOp toMIR(BinaryOp op) {
        switch (op) {
        case BinaryOp::ADD:
            return middleend::mir::BinaryOp::ADD;
        case BinaryOp::SUB:
            return middleend::mir::BinaryOp::SUB;
        case BinaryOp::MUL:
            return middleend::mir::BinaryOp::MUL;
        case BinaryOp::AND:
            return middleend::mir::BinaryOp::AND;
        }
    }
} // namespace middleend::mir
