#include <string>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    Function::Function(std::unique_ptr<AtomIdentifier> name,
                       std::unique_ptr<Scope> body)
        : name(std::move(name)), body(std::move(body)) {}

    std::unique_ptr<AtomIdentifier> &Function::getName() { return name; }

    std::unique_ptr<Scope> &Function::getBody() { return body; }

    std::string Function::toString(SymbolTable &symbol_table) {
        std::string type_str = ::frontend::toString(name->getType());
        std::string name_str = name->toString(symbol_table);

        std::string res = type_str + " " + name_str + "()";
        ToStringVisitor visitor = ToStringVisitor(symbol_table);
        body->accept(&visitor);
        res += " " + visitor.getResult();

        return res;
    }

    Program::Program(std::vector<Function> functions, SymbolTable &symbol_table)
        : functions(std::move(functions)), symbol_table(symbol_table) {}

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

    ToStringVisitor::ToStringVisitor(SymbolTable &symbol_table)
        : symbol_table(symbol_table), prefix(""), res("") {}

    std::string ToStringVisitor::getResult() { return res; }

    void ToStringVisitor::convertSubexpression(Instruction *i) {
        bool isScope = dynamic_cast<Scope *>(i);
        std::string old_prefix = prefix;

        if (!isScope)
            prefix += "    ";
        i->accept(this);
        if (!isScope)
            prefix = old_prefix;

        std::string subexpr_res;
        std::string inner = getResult();
        if (isScope) {
            subexpr_res += ' ' + inner.substr(inner.find_first_not_of(' '));
        } else {
            subexpr_res += '\n' + inner;
        }

        res = subexpr_res;
    }

    void ToStringVisitor::visit(Instruction *i) {
        res = prefix + "[UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(Scope *s) {
        std::string scope_res = "{\n";

        std::string old_prefix = prefix;
        prefix += "    ";
        for (auto &i : s->getInstructions()) {
            i->accept(this);
            scope_res += getResult() + "\n";
        }
        prefix = old_prefix;

        scope_res += prefix + "}";
        res = prefix + scope_res;
    }

    void ToStringVisitor::visit(InstructionDeclaration *i) {
        const std::string type = toString(i->getVariable()->getType());
        const std::string variable = i->getVariable()->toString(symbol_table);

        res = prefix + type + " " + variable + ";";
    }

    void ToStringVisitor::visit(InstructionDeclarationAssignValue *i) {
        const std::string type = toString(i->getVariable()->getType());
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

        res = prefix + target + '(' + ");";
    }

    void ToStringVisitor::visit(InstructionCallAssign *i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string target = i->getTarget()->toString(symbol_table);

        res = prefix + variable + " = " + target + '(' + ");";
    }

    void ToStringVisitor::visit(InstructionIf *i) {
        const std::string cond = i->getCond()->toString(symbol_table);
        std::string if_res = prefix + "if (" + cond + ")";

        convertSubexpression(i->getTBranch().get());
        std::string t_branch = getResult();
        if_res += t_branch;

        if (i->getFBranch() != nullptr) {
            convertSubexpression(i->getFBranch().get());
            std::string f_branch = getResult();
            if_res += dynamic_cast<Scope *>(i->getTBranch().get()) != nullptr
                          ? " "
                          : "\n" + prefix;
            if_res += "else";
            if_res += f_branch;
        }

        res = if_res;
    }

    void ToStringVisitor::visit(InstructionWhile *i) {
        const std::string cond = i->getCond()->toString(symbol_table);
        std::string if_res = prefix + "while (" + cond + ")";

        convertSubexpression(i->getBody().get());
        std::string body = getResult();
        if_res += body;

        res = if_res;
    }
} // namespace frontend::ast
