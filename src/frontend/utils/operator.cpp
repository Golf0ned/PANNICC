#include "frontend/utils/operator.h"

#include <utility>

using namespace frontend;

bool isAssignment(BinaryOp op) {
    switch (op) {
    case BinaryOp::ASSIGN:
    case BinaryOp::ADD_ASSIGN:
    case BinaryOp::SUB_ASSIGN:
    case BinaryOp::MUL_ASSIGN:
    case BinaryOp::DIV_ASSIGN:
    case BinaryOp::AND_ASSIGN:
    case BinaryOp::OR_ASSIGN:
    case BinaryOp::XOR_ASSIGN:
    case BinaryOp::LSHIFT_ASSIGN:
    case BinaryOp::RSHIFT_ASSIGN:
        return false;
    default:
        return true;
    }
    std::unreachable();
}

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
        {"=", BinaryOp::ASSIGN},
        {"+=", BinaryOp::ADD},
        {"-=", BinaryOp::SUB},
        {"*=", BinaryOp::MUL},
        {"/=", BinaryOp::DIV},
        {"&=", BinaryOp::AND},
        {"|=", BinaryOp::OR},
        {"^=", BinaryOp::XOR},
        {"<<=", BinaryOp::LSHIFT},
        {">>=", BinaryOp::RSHIFT},
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
    case BinaryOp::ASSIGN:
        return "=";
    case BinaryOp::ADD_ASSIGN:
        return "+=";
    case BinaryOp::SUB_ASSIGN:
        return "-=";
    case BinaryOp::MUL_ASSIGN:
        return "*=";
    case BinaryOp::DIV_ASSIGN:
        return "/=";
    case BinaryOp::AND_ASSIGN:
        return "&=";
    case BinaryOp::OR_ASSIGN:
        return "|=";
    case BinaryOp::XOR_ASSIGN:
        return "^=";
    case BinaryOp::LSHIFT_ASSIGN:
        return "<<=";
    case BinaryOp::RSHIFT_ASSIGN:
        return ">>=";
    }
    std::unreachable();
}

middleend::mir::BinaryOp toMir(BinaryOp op) {
    switch (op) {
    case BinaryOp::ADD:
    case BinaryOp::ADD_ASSIGN:
        return middleend::mir::BinaryOp::ADD;
    case BinaryOp::SUB:
    case BinaryOp::SUB_ASSIGN:
        return middleend::mir::BinaryOp::SUB;
    case BinaryOp::MUL:
    case BinaryOp::MUL_ASSIGN:
        return middleend::mir::BinaryOp::MUL;
    case BinaryOp::DIV:
    case BinaryOp::DIV_ASSIGN:
        return middleend::mir::BinaryOp::SDIV;
    case BinaryOp::AND:
    case BinaryOp::AND_ASSIGN:
        return middleend::mir::BinaryOp::AND;
    case BinaryOp::OR:
    case BinaryOp::OR_ASSIGN:
        return middleend::mir::BinaryOp::OR;
    case BinaryOp::XOR:
    case BinaryOp::XOR_ASSIGN:
        return middleend::mir::BinaryOp::XOR;
    case BinaryOp::LSHIFT:
    case BinaryOp::LSHIFT_ASSIGN:
        return middleend::mir::BinaryOp::SHL;
    case BinaryOp::RSHIFT:
    case BinaryOp::RSHIFT_ASSIGN:
        return middleend::mir::BinaryOp::ASHR;
    case BinaryOp::ASSIGN:
        std::unreachable();
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
