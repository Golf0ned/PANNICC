#include "frontend/utils/operator.h"

#include <utility>

using namespace frontend;

BinaryOp strToBinaryOp(std::string str) {
    static const std::unordered_map<std::string, BinaryOp> fromStr = {
        // clang-format off
        {"+", BinaryOp::ADD},
        {"-", BinaryOp::SUB},
        {"*", BinaryOp::MUL},
        {"/", BinaryOp::DIV},
        {"&", BinaryOp::AND},
        {"|", BinaryOp::OR},
        {"^", BinaryOp::XOR},
        {"<<", BinaryOp::LSHIFT},
        {">>", BinaryOp::RSHIFT},
        // clang-format on
    };

    return fromStr.at(str);
}

std::string toString(BinaryOp op) {
    switch (op) {
    case BinaryOp::ADD:
        return "+";
    case BinaryOp::SUB:
        return "-";
    case BinaryOp::MUL:
        return "*";
    case BinaryOp::DIV:
        return "/";
    case BinaryOp::AND:
        return "&";
    case BinaryOp::OR:
        return "|";
    case BinaryOp::XOR:
        return "^";
    case BinaryOp::LSHIFT:
        return "<<";
    case BinaryOp::RSHIFT:
        return ">>";
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
    case BinaryOp::OR:
        return middleend::mir::BinaryOp::OR;
    case BinaryOp::XOR:
        return middleend::mir::BinaryOp::XOR;
    case BinaryOp::LSHIFT:
        return middleend::mir::BinaryOp::SHL;
    case BinaryOp::RSHIFT:
        return middleend::mir::BinaryOp::ASHR;
    }
    std::unreachable();
}

UnaryOp strToUnaryOp(std::string str) {
    static const std::unordered_map<std::string, UnaryOp> fromStr = {
        // clang-format off
        {"+", UnaryOp::PLUS},
        {"-", UnaryOp::MINUS},
        {"~", UnaryOp::NOT},
        {"*", UnaryOp::DEREF},
        {"&", UnaryOp::ADDRESS},
        // clang-format on
    };

    return fromStr.at(str);
}

std::string toString(UnaryOp op) {
    switch (op) {
    case UnaryOp::PLUS:
        return "+";
    case UnaryOp::MINUS:
        return "-";
    case UnaryOp::NOT:
        return "~";
    case UnaryOp::DEREF:
        return "*";
    case UnaryOp::ADDRESS:
        return "&";
    }
    std::unreachable();
}
