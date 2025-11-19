#include "backend/lir/lir.h"

namespace backend::lir {
    Program::Program(std::list<std::unique_ptr<Instruction>> instructions)
        : instructions(std::move(instructions)) {}

    std::list<std::unique_ptr<Instruction>> &Program::getInstructions() {
        return instructions;
    }

    std::string Program::toString() {
        ToStringVisitor tsv;
        for (auto &i : instructions)
            i->accept(&tsv);
        return tsv.getResult();
    }

    std::string ToStringVisitor::getResult() { return result; }

    void ToStringVisitor::visit(Instruction *i) {}

    void ToStringVisitor::visit(Label *l) {
        if (!result.empty())
            result += "\n\n";
        result += l->getName() + ':';
    }

    void ToStringVisitor::visit(InstructionMov *i) {
        if (!result.empty())
            result += '\n';
        result += "mov [TODO]";
    }

    void ToStringVisitor::visit(InstructionPush *i) {
        if (!result.empty())
            result += '\n';
        result += "push [TODO]";
    }

    void ToStringVisitor::visit(InstructionPop *i) {
        if (!result.empty())
            result += '\n';
        result += "pop [TODO]";
    }

    void ToStringVisitor::visit(InstructionCmp *i) {
        if (!result.empty())
            result += '\n';
        result += "cmp [TODO]";
    }

    void ToStringVisitor::visit(InstructionJmp *i) {
        if (!result.empty())
            result += '\n';
        result += "jmp [TODO]";
    }

    void ToStringVisitor::visit(InstructionCJmp *i) {
        if (!result.empty())
            result += '\n';
        result += "cjmp [TODO]";
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        if (!result.empty())
            result += '\n';
        result += "call [TODO]";
    }

    void ToStringVisitor::visit(InstructionRet *i) {
        if (!result.empty())
            result += '\n';
        result += "ret [TODO]";
    }

    void ToStringVisitor::visit(InstructionPhi *i) {
        if (!result.empty())
            result += '\n';
        result += "phi [TODO]";
    }

    void ToStringVisitor::visit(InstructionVirtual *i) {
        i->getInstruction()->accept(this);
        result += "    # [virtual]";
    }

    void ToStringVisitor::visit(InstructionUnknown *i) {
        if (!result.empty())
            result += '\n';
        result += "[unknown]";
    }
} // namespace backend::lir
