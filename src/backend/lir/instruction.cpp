#include <utility>

#include "backend/lir/instruction.h"

namespace backend::lir {
    Label::Label(std::string name) : name(name) {}

    std::string Label::getName() { return name; }

    void Label::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionMov::InstructionMov(Extend extend, DataSize src_size,
                                   DataSize dst_size, Operand *src,
                                   Operand *dst)
        : extend(extend), src_size(src_size), dst_size(dst_size), src(src),
          dst(dst) {}

    Extend InstructionMov::getExtend() { return extend; }

    DataSize InstructionMov::getSrcSize() { return src_size; }

    DataSize InstructionMov::getDstSize() { return dst_size; }

    Operand *InstructionMov::getSrc() { return src; }

    Operand *InstructionMov::getDst() { return dst; }

    void InstructionMov::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPush::InstructionPush(DataSize size, Operand *src)
        : size(size), src(src) {}

    DataSize InstructionPush::getSize() { return size; }

    Operand *InstructionPush::getSrc() { return src; }

    void InstructionPush::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPop::InstructionPop(DataSize size, Operand *dst)
        : size(size), dst(dst) {}

    DataSize InstructionPop::getSize() { return size; }

    Operand *InstructionPop::getDst() { return dst; }

    void InstructionPop::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionConvert::InstructionConvert(DataSize from, DataSize to)
        : from(from), to(to) {}

    DataSize InstructionConvert::getFrom() { return from; }

    DataSize InstructionConvert::getTo() { return to; }

    void InstructionConvert::accept(InstructionVisitor *v) { v->visit(this); }

    BinaryOp fromMir(middleend::mir::BinaryOp op) {
        switch (op) {
        case middleend::mir::BinaryOp::ADD:
            return BinaryOp::ADD;
        case middleend::mir::BinaryOp::SUB:
            return BinaryOp::SUB;
        case middleend::mir::BinaryOp::MUL:
            return BinaryOp::IMUL;
        case middleend::mir::BinaryOp::SDIV:
            return BinaryOp::IDIV;
        case middleend::mir::BinaryOp::AND:
            return BinaryOp::AND;
        case middleend::mir::BinaryOp::OR:
            return BinaryOp::OR;
        case middleend::mir::BinaryOp::XOR:
            return BinaryOp::XOR;
        case middleend::mir::BinaryOp::SHL:
            return BinaryOp::SHL;
        case middleend::mir::BinaryOp::ASHR:
            return BinaryOp::SAR;
        }
        std::unreachable();
    }

    std::string toString(BinaryOp op) {
        switch (op) {
        case BinaryOp::ADD:
            return "add";
        case BinaryOp::SUB:
            return "sub";
        case BinaryOp::IMUL:
            return "imul";
        case BinaryOp::IDIV:
            return "idiv";
        case BinaryOp::AND:
            return "and";
        case BinaryOp::OR:
            return "or";
        case BinaryOp::XOR:
            return "xor";
        case BinaryOp::SHL:
            return "shl";
        case BinaryOp::SAR:
            return "sar";
        }
        std::unreachable();
    }

    InstructionBinaryOp::InstructionBinaryOp(BinaryOp op, DataSize size,
                                             Operand *src, Operand *dst)
        : op(op), size(size), src(src), dst(dst) {}

    BinaryOp InstructionBinaryOp::getOp() { return op; }

    DataSize InstructionBinaryOp::getSize() { return size; }

    Operand *InstructionBinaryOp::getSrc() { return src; }

    Operand *InstructionBinaryOp::getDst() { return dst; }

    void InstructionBinaryOp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionSpecialOp::InstructionSpecialOp(BinaryOp op, DataSize size,
                                               Operand *src)
        : op(op), size(size), src(src) {}

    BinaryOp InstructionSpecialOp::getOp() { return op; }

    DataSize InstructionSpecialOp::getSize() { return size; }

    Operand *InstructionSpecialOp::getSrc() { return src; }

    void InstructionSpecialOp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionLea::InstructionLea(DataSize size, Address *src, Operand *dst)
        : size(size), src(src), dst(dst) {}

    DataSize InstructionLea::getSize() { return size; }

    Address *InstructionLea::getSrc() { return src; }

    Operand *InstructionLea::getDst() { return dst; }

    void InstructionLea::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCmp::InstructionCmp(DataSize size, Operand *src_1,
                                   Operand *src_2)
        : size(size), src_1(src_1), src_2(src_2) {}

    DataSize InstructionCmp::getSize() { return size; }

    Operand *InstructionCmp::getSrc1() { return src_1; }

    Operand *InstructionCmp::getSrc2() { return src_2; }

    void InstructionCmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionJmp::InstructionJmp(std::string label) : label(label) {}

    std::string InstructionJmp::getLabel() { return label; }

    void InstructionJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCJmp::InstructionCJmp(ConditionCode cmp, std::string label)
        : cmp(cmp), label(label) {}

    ConditionCode InstructionCJmp::getCmp() { return cmp; }

    std::string InstructionCJmp::getLabel() { return label; }

    void InstructionCJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCall::InstructionCall(std::string label) : label(label) {}

    std::string InstructionCall::getLabel() { return label; }

    void InstructionCall::accept(InstructionVisitor *v) { v->visit(this); }

    void InstructionRet::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPhi::InstructionPhi(std::list<Operand *> src, Operand *dst)
        : src(std::move(src)), dst(dst) {}

    std::list<Operand *> &InstructionPhi::getSrc() { return src; }

    Operand *InstructionPhi::getDst() { return dst; }

    void InstructionPhi::accept(InstructionVisitor *v) { v->visit(this); }

    void InstructionUnknown::accept(InstructionVisitor *v) { v->visit(this); }
} // namespace backend::lir
