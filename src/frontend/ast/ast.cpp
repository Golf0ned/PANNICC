#include <string>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    Function::Function(Type type, AtomIdentifier *name, Scope *body)
        : type(type), name(name), body(body) {}

    Type Function::getType() { return type; }

    AtomIdentifier *Function::getName() { return name; }

    Scope *Function::getBody() { return body; }

    std::string Function::toString(SymbolTable &symbol_table) {
        std::string type_str = ::frontend::toString(type);
        std::string name_str = name->toString(symbol_table);

        std::string res = type_str + " " + name_str + "()";
        ToStringVisitor visitor = ToStringVisitor(symbol_table);
        body->accept(&visitor);
        res += " " + visitor.getResult();

        return res;
    }

    Program::Program(std::vector<Function> functions, SymbolTable &symbol_table)
        : functions(functions), symbol_table(symbol_table) {}

    std::vector<Function> &Program::getFunctions() { return functions; }

    SymbolTable &Program::getSymbolTable() { return symbol_table; }

    std::string Program::toString() {
        std::string res = "";

        for (auto iter = functions.begin(); iter != functions.end(); iter++) {
            if (iter != functions.begin())
                res += "\n\n";
            res += iter->toString(symbol_table);
        }

        return res;
    }

    Program::~Program() {
        for (Function &f : functions) {
            delete f.getBody();
            delete f.getName();
        }
    }

    ToStringVisitor::ToStringVisitor(SymbolTable &symbol_table)
        : symbol_table(symbol_table), prefix(""), res("") {}

    std::string ToStringVisitor::getResult() { return res; }

    void ToStringVisitor::visit(Instruction *i) {
        res = prefix + "[UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(Scope *s) {
        std::string scope_res = "{\n";

        std::string old_prefix = prefix;
        prefix += "    ";
        for (Instruction *i : s->getInstructions()) {
            i->accept(this);
            scope_res += getResult() + "\n";
        }
        prefix = old_prefix;

        scope_res += prefix + "}";
        res = prefix + scope_res;
    }

    void ToStringVisitor::visit(InstructionDeclaration *i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(symbol_table);

        res = prefix + type + " " + variable + ";";
    }

    void ToStringVisitor::visit(InstructionDeclarationAssignValue *i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string value = i->getValue()->toString(symbol_table);

        res = prefix + type + " " + variable + " = " + value + ";";
    }

    void ToStringVisitor::visit(InstructionAssignValue *i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string value = i->getValue()->toString(symbol_table);

        res = prefix + variable + " = " + value + ";";
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp *i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string op = toString(i->getOp());
        const std::string left = i->getLeft()->toString(symbol_table);
        const std::string right = i->getRight()->toString(symbol_table);

        res = prefix + variable + " = " + left + " " + op + " " + right + ";";
    }

    void ToStringVisitor::visit(InstructionReturn *i) {
        const std::string value = i->getValue()->toString(symbol_table);

        res = prefix + "return " + value + ";";
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        const std::string target = i->getTarget()->toString(symbol_table);

        res = prefix + "CALL " + target + "();";
    }

    void ToStringVisitor::visit(InstructionCallAssign *i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string target = i->getTarget()->toString(symbol_table);

        res = prefix + variable + " = CALL " + target + "();";
    }
} // namespace frontend::ast
