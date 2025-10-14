#include <utility>

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
        case BinaryOp::DIV:
            return "*";
        case BinaryOp::AND:
            return "&";
        }
        std::unreachable();
    }

    std::string toString(UnaryOp op) {
        switch (op) {
        case UnaryOp::PLUS:
            return "+";
        case UnaryOp::MINUS:
            return "-";
        }
        std::unreachable();
    }

    middleend::mir::BinaryOp toMir(BinaryOp op) {
        switch (op) {
        case BinaryOp::ADD:
            return middleend::mir::BinaryOp::ADD;
        case BinaryOp::SUB:
            return middleend::mir::BinaryOp::SUB;
        case BinaryOp::MUL:
            return middleend::mir::BinaryOp::MUL;
        case BinaryOp::DIV:
            return middleend::mir::BinaryOp::SDIV;
        case BinaryOp::AND:
            return middleend::mir::BinaryOp::AND;
        }
        std::unreachable();
    }
} // namespace frontend
