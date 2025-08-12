#include "middleend/mir/operator.h"


namespace frontend {
    std::string toString(BinaryOp op) {
        switch (op) {
            case BinaryOp::ADD: return "add";
            case BinaryOp::SUB: return "sub";
            case BinaryOp::MUL: return "mul";
            case BinaryOp::AND: return "and";
            default:            return "UNKNOWN";
        }
    }
}
