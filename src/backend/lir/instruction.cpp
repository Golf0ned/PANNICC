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

    InstructionCall::InstructionCall(std::string label) : label(label) {}

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    void InstructionRet::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace backend::lir
