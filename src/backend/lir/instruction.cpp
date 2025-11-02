#include "backend/lir/instruction.h"

namespace backend::lir {
    Label::Label(std::string name) : name(name) {}

    std::string Label::getName() { return name; }

    void Label::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionMov::InstructionMov(DataSize size, Operand *dst, Operand *src)
        : size(size), dst(dst), src(src) {}

    void InstructionMov::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPush::InstructionPush(DataSize size, Operand *src)
        : size(size), src(src) {}

    void InstructionPush::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionPop::InstructionPop(DataSize size, Operand *dst)
        : size(size), dst(dst) {}

    void InstructionPop::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCmp::InstructionCmp(Operand *src_1, Operand *src_2)
        : src_1(src_1), src_2(src_2) {}

    void InstructionCmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionJmp::InstructionJmp(std::string label) : label(label) {}

    void InstructionJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCJmp::InstructionCJmp(ConditionCode cmp, std::string label)
        : cmp(cmp), label(label) {}

    void InstructionCJmp::accept(InstructionVisitor *v) { v->visit(this); }

    InstructionCall::InstructionCall(std::string label) : label(label) {}

    void InstructionCall::accept(InstructionVisitor *v) { v->visit(this); }

    void InstructionRet::accept(InstructionVisitor *v) { v->visit(this); }
} // namespace backend::lir
