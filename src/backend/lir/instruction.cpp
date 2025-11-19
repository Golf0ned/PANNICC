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

    void InstructionMov::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPush::InstructionPush(DataSize size, Operand *src)
        : size(size), src(src) {}

    void InstructionPush::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPop::InstructionPop(DataSize size, Operand *dst)
        : size(size), dst(dst) {}

    void InstructionPop::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCmp::InstructionCmp(DataSize size, Operand *src_1,
                                   Operand *src_2)
        : size(size), src_1(src_1), src_2(src_2) {}

    void InstructionCmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionJmp::InstructionJmp(std::string label) : label(label) {}

    void InstructionJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCJmp::InstructionCJmp(ConditionCode cmp, std::string label)
        : cmp(cmp), label(label) {}

    void InstructionCJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCall::InstructionCall(std::string label) : label(label) {}

    void InstructionCall::accept(InstructionVisitor *v) { v->visit(this); }

    void InstructionRet::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPhi::InstructionPhi(std::list<Operand *> src, Operand *dst)
        : src(std::move(src)), dst(dst) {}

    void InstructionPhi::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionVirtual::InstructionVirtual(std::unique_ptr<Instruction> i)
        : instruction(std::move(i)) {}

    std::unique_ptr<Instruction> &InstructionVirtual::getInstruction() {
        return instruction;
    }

    void InstructionVirtual::accept(InstructionVisitor *v) { v->visit(this); }

    void InstructionUnknown::accept(InstructionVisitor *v) { v->visit(this); }

} // namespace backend::lir
