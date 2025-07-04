#include <string>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"


namespace frontend::AST {
    void Program::addInstruction(Instruction* i) {
        instructions.push_back(i);
    }

    std::string Program::toString() {
        std::string res = "";

        ToStringVisitor visitor = ToStringVisitor();
        for (Instruction* i : instructions) {
            i->accept(&visitor);
            res += visitor.getResult() + "\n";
        }
        return res;
    }

    Program::~Program() {
        for (Instruction* i : instructions) {
            delete i;
        }
    }

    ToStringVisitor::ToStringVisitor() : res("") {}

    std::string ToStringVisitor::getResult() {
        return res;
    }

    void ToStringVisitor::visit(Atom* a) {
        res = "[UNKNOWN ATOM]";
    }

    void ToStringVisitor::visit(AtomIdentifier* a) {
        std::string value = a->getValue();

        res = value;
    }

    void ToStringVisitor::visit(AtomLiteral* a) {
        uint64_t value = a->getValue();

        res = std::to_string(value);
    }

    void ToStringVisitor::visit(Instruction* i) {
        res = "[UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(InstructionDeclaration* i) {
        const std::string type = toString(i->getType());
        
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        res = "DECLARE " + type + " " + variable;
    }

    void ToStringVisitor::visit(InstructionAssignValue* i) {
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        i->getValue()->accept(this);
        const std::string value = this->getResult();

        res = "ASSIGN " + variable + " = " + value;
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp* i) {
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        const std::string op = toString(i->getOp());

        i->getLeft()->accept(this);
        const std::string left = this->getResult();

        i->getRight()->accept(this);
        const std::string right = this->getResult();

        res = "ASSIGN " + variable + " = " + left + " " + op + " " + right;
    }

    void ToStringVisitor::visit(InstructionReturn* i) {
        i->getValue()->accept(this);
        const std::string value = this->getResult();

        res = "RETURN " + value;
    }
}
