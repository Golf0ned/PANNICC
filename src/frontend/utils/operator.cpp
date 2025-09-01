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
} // namespace frontend
