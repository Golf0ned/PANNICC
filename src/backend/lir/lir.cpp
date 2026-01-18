#include "backend/lir/lir.h"

namespace backend::lir {
    Function::Function(std::string name, uint64_t num_params,
                       uint64_t stack_bytes,
                       std::list<std::unique_ptr<Instruction>> instructions)
        : name(name), num_params(num_params), stack_bytes(stack_bytes),
          instructions(std::move(instructions)) {}

    std::string Function::getName() { return name; };

    uint64_t Function::getNumParams() { return num_params; }

    uint64_t Function::getStackBytes() { return stack_bytes; }

    std::list<std::unique_ptr<Instruction>> &Function::getInstructions() {
        return instructions;
    }

    std::string Function::toString() {
        // TODO: braces? some sort of grouping?

        std::string header = name + "(" + std::to_string(num_params) + ", " +
                             std::to_string(stack_bytes) + ")\n";

        ToStringVisitor tsv;
        for (auto &i : instructions)
            i->accept(&tsv);

        return header + tsv.getResult();
    }

    Program::Program(std::list<std::unique_ptr<Function>> functions,
                     std::unique_ptr<OperandManager> om)
        : functions(std::move(functions)), om(std::move(om)) {}

    std::list<std::unique_ptr<Function>> &Program::getFunctions() {
        return functions;
    }

    OperandManager *Program::getOm() { return om.get(); }

    std::string Program::toString() {
        std::string res = "";
        for (auto iter = functions.begin(); iter != functions.end(); iter++) {
            if (iter != functions.begin())
                res += "\n\n";
            res += (*iter)->toString();
        }
        return res;
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
            result += "\n        ";

        result += "mov";

        auto extend = i->getExtend();
        if (extend != Extend::NONE) {
            result += toChar(extend);
            result += toChar(i->getSrcSize());
            result += toChar(i->getDstSize());
            result += "  ";
        } else {
            result += toChar(i->getDstSize());
            result += "    ";
        }

        result += i->getSrc()->toString() + ", " + i->getDst()->toString();
    }

    void ToStringVisitor::visit(InstructionPush *i) {
        if (!result.empty())
            result += "\n        ";
        result += "push";
        result += toChar(i->getSize());
        result += "   " + i->getSrc()->toString();
    }

    void ToStringVisitor::visit(InstructionPop *i) {
        if (!result.empty())
            result += "\n        ";
        result += "pop";
        result += toChar(i->getSize());
        result += "    " + i->getDst()->toString();
    }

    void ToStringVisitor::visit(InstructionConvert *i) {
        if (!result.empty())
            result += "\n        ";

        auto from = i->getFrom();
        auto to = i->getTo();

        result += 'c';
        result += toChar(from);
        result += 't';
        if (from == to)
            if (to == DataSize::QUADWORD)
                result += 'o';
            else
                result += 'd';
        else
            result += toChar(i->getTo());
    }

    void ToStringVisitor::visit(InstructionBinaryOp *i) {
        if (!result.empty())
            result += "\n        ";
        auto op_str = toString(i->getOp());
        result += op_str + toChar(i->getSize());
        result += std::string(8 - op_str.size() - 1, ' ');
        result += i->getSrc()->toString() + ", " + i->getDst()->toString();
    }

    void ToStringVisitor::visit(InstructionSpecialOp *i) {
        if (!result.empty())
            result += "\n        ";
        auto op_str = toString(i->getOp());
        result += op_str + toChar(i->getSize());
        result += std::string(8 - op_str.size() - 1, ' ');
        result += i->getSrc()->toString();
    }

    void ToStringVisitor::visit(InstructionLea *i) {
        if (!result.empty())
            result += "\n        ";
        result += "lea";
        result += toChar(i->getSize());
        result +=
            "    " + i->getSrc()->toString() + ", " + i->getDst()->toString();
    }

    void ToStringVisitor::visit(InstructionCmp *i) {
        if (!result.empty())
            result += "\n        ";
        result += "cmp";
        result += toChar(i->getSize());
        result +=
            "    " + i->getSrc1()->toString() + ", " + i->getSrc2()->toString();
    }

    void ToStringVisitor::visit(InstructionJmp *i) {
        if (!result.empty())
            result += "\n        ";
        result += "jmp     " + i->getLabel();
    }

    void ToStringVisitor::visit(InstructionCJmp *i) {
        if (!result.empty())
            result += "\n        ";
        auto cc_str = toString(i->getCmp());
        result += "j" + cc_str;
        result += std::string(8 - cc_str.size() - 1, ' ');
        result += i->getLabel();
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        if (!result.empty())
            result += "\n        ";
        result += "call    " + i->getLabel();
    }

    void ToStringVisitor::visit(InstructionRet *i) {
        if (!result.empty())
            result += "\n        ";
        result += "ret";
    }

    void ToStringVisitor::visit(InstructionVirtualCall *i) {
        if (!result.empty())
            result += "\n        ";
        result += "call    " + i->getLabel() + "(";

        auto &args = i->getArgs();
        for (auto arg = args.begin(); arg != args.end(); arg++) {
            if (arg != args.begin())
                result += ", ";
            result += (*arg)->toString();
        }

        result += ')';
    }

    void ToStringVisitor::visit(InstructionUnknown *i) {
        if (!result.empty())
            result += "\n        ";
        result += "unknown";
    }
} // namespace backend::lir
