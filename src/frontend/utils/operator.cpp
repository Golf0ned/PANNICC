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
        case BinaryOp::AND:
            return "&";
        }
        std::unreachable();
    }

    std::string toString(CmpOp op) {
        switch (op) {
        case CmpOp::GT:
            return ">";
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
        case BinaryOp::AND:
            return middleend::mir::BinaryOp::AND;
        }
        std::unreachable();
    }

    middleend::mir::CmpOp toMir(CmpOp op, bool isSigned) {
        if (isSigned) {
            switch (op) {
            case CmpOp::GT:
                return middleend::mir::CmpOp::SGT;
            }
            std::unreachable();
        } else {
            switch (op) {
            case CmpOp::GT:
                std::unreachable();
            }
            std::unreachable();
        }
    }
} // namespace frontend
