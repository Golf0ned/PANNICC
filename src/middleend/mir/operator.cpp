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
        case BinaryOp::AND:
            return "and";
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
