#include "backend/lir/instruction.h"

namespace backend::lir {
    Label::Label(std::string name) : name(name) {}

    std::string Label::getName() { return name; }

    InstructionMov::InstructionMov(DataSize size, Operand *dst, Operand *src)
        : size(size), dst(dst), src(src) {}

    void InstructionMov::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionPush::InstructionPush(DataSize size, Operand *src)
        : size(size), src(src) {}

    void InstructionPush::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionPop::InstructionPop(DataSize size, Operand *dst)
        : size(size), dst(dst) {}

    void InstructionPop::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCmp::InstructionCmp(Operand *src_1, Operand *src_2)
        : src_1(src_1), src_2(src_2) {}

    void InstructionCmp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionJmp::InstructionJmp(std::string label) : label(label) {}

    void InstructionJmp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCJmp::InstructionCJmp(ConditionCode cmp, std::string label)
        : cmp(cmp), label(label) {}

    void InstructionCJmp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCall::InstructionCall(std::string label) : label(label) {}

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    void InstructionRet::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace backend::lir
