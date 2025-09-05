#include "frontend/utils/operator.h"

namespace frontend {
    std::string toString(BinaryOp op) {
        switch (op) {
        case BinaryOp::ADD:
            return "+";
        case BinaryOp::SUB:
            return "-";
        case BinaryOp::MUL:
            return "*";
        case BinaryOp::AND:
            return "&";
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
} // namespace frontend
