#include <string>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"


namespace frontend::ast {
    Function::Function(Type type, AtomIdentifier* name, Scope* body)
        : type(type), name(name), body(body) {}

    Type Function::getType() {
        return type;
    }

    AtomIdentifier* Function::getName() {
        return name;
    }

    Scope* Function::getBody() {
        return body;
    }

    std::string Function::toString() {
        std::string res = ::frontend::toString(type) + " " + name->getValue() + "()";
        ToStringVisitor visitor = ToStringVisitor();
        body->accept(&visitor);
        res += " " + visitor.getResult();
        return res;
    }


    void Program::addFunction(Function f) {
        functions.push_back(f);
    }

    std::string Program::toString() {
        std::string res = "";

        for (Function& f : functions) {
            res += f.toString() + "\n";
        }
        return res;
    }

    Program::~Program() {
        for (Function& f : functions) {
            delete f.getBody();
            delete f.getName();
        }
    }


    ToStringVisitor::ToStringVisitor() : res(""), prefix("") {}

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
        res = prefix + "[UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(Scope* s) {
        std::string scope_res = "{\n";

        this->prefix += "    ";
        for (Instruction* i : s->getInstructions()) {
            i->accept(this);
            scope_res += this->getResult() + "\n";
        }
        this->prefix = this->prefix.substr(0, this->prefix.size() - 4);

        scope_res += prefix + "}";
        res = prefix + scope_res;
    }

    void ToStringVisitor::visit(InstructionDeclaration* i) {
        const std::string type = toString(i->getType());
        
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        res = prefix + "DECLARE " + type + " " + variable;
    }

    void ToStringVisitor::visit(InstructionAssignValue* i) {
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        i->getValue()->accept(this);
        const std::string value = this->getResult();

        res = prefix + "ASSIGN " + variable + " = " + value;
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp* i) {
        i->getVariable()->accept(this);
        const std::string variable = this->getResult();

        const std::string op = toString(i->getOp());

        i->getLeft()->accept(this);
        const std::string left = this->getResult();

        i->getRight()->accept(this);
        const std::string right = this->getResult();

        res = prefix + "ASSIGN " + variable + " = " + left + " " + op + " " + right;
    }

    void ToStringVisitor::visit(InstructionReturn* i) {
        i->getValue()->accept(this);
        const std::string value = this->getResult();

        res = prefix + "RETURN " + value;
    }
}
