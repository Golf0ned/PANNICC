#include <string>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    Function::Function(Type type, std::unique_ptr<AtomIdentifier> name,
                       std::vector<Parameter> parameters,
                       std::unique_ptr<Scope> body)
        : type(type), name(std::move(name)), parameters(std::move(parameters)),
          body(std::move(body)) {}

    Type Function::getType() { return type; }

    std::unique_ptr<AtomIdentifier> &Function::getName() { return name; }

    std::vector<Parameter> &Function::getParameters() { return parameters; }

    std::unique_ptr<Scope> &Function::getBody() { return body; }

    std::string Function::toString(SymbolTable *symbol_table) {
        std::string type_str = ::frontend::toString(type);
        std::string name_str = name->toString(*symbol_table);

        std::string res = type_str + " " + name_str + '(';
        for (auto iter = parameters.begin(); iter != parameters.end(); iter++) {
            if (iter != parameters.begin())
                res += ", ";
            auto &[param_type, param_name] = *iter;
            res += ::frontend::toString(param_type) + ' ' +
                   param_name->toString(*symbol_table);
        }
        res += ')';

        ToStringVisitor visitor = ToStringVisitor(symbol_table);
        body->accept(&visitor);
        res += " " + visitor.getResult();

        return res;
    }

    Program::Program(std::vector<Function> functions,
                     std::unique_ptr<SymbolTable> symbol_table)
        : functions(std::move(functions)),
          symbol_table(std::move(symbol_table)) {}

    std::vector<Function> &Program::getFunctions() { return functions; }

    std::unique_ptr<SymbolTable> &Program::getSymbolTable() {
        return symbol_table;
    }

    std::string Program::toString() {
        std::string res = "";

        for (auto iter = functions.begin(); iter != functions.end(); iter++) {
            if (iter != functions.begin())
                res += "\n\n";
            res += iter->toString(symbol_table.get());
        }

        return res;
    }

    ToStringVisitor::ToStringVisitor(SymbolTable *symbol_table)
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

    void ToStringVisitor::visit(InstructionExpr *i) {
        i->getExpr()->accept(this);
        res = prefix + res + ';';
    }

    void ToStringVisitor::visit(InstructionDeclaration *i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(*symbol_table);

        res = prefix + type + " " + variable + ";";
    }

    void ToStringVisitor::visit(InstructionDeclarationAssign *i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(*symbol_table);
        i->getValue()->accept(this);

        res = prefix + type + " " + variable + " = " + res + ";";
    }

    void ToStringVisitor::visit(InstructionAssign *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        i->getValue()->accept(this);

        res = prefix + variable + " = " + res + ";";
    }

    void ToStringVisitor::visit(InstructionOpAssign *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        const std::string op = toString(i->getOp());
        i->getValue()->accept(this);

        res = prefix + variable + ' ' + op + "= " + res + ";";
    }

    void ToStringVisitor::visit(InstructionReturn *i) {
        i->getValue()->accept(this);
        res = prefix + "return " + res + ";";
    }

    void ToStringVisitor::visit(InstructionIf *i) {
        i->getCond()->accept(this);
        const std::string cond = res;
        std::string if_res = prefix + "if (" + cond + ")";

        convertSubexpression(i->getTBranch().get());
        std::string t_branch = getResult();
        if_res += t_branch;

        if (i->hasFBranch()) {
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
        i->getCond()->accept(this);
        const std::string cond = res;
        std::string while_res = prefix + "while (" + cond + ")";

        convertSubexpression(i->getBody().get());
        std::string body = getResult();
        while_res += body;

        res = while_res;
    }

    void ToStringVisitor::visit(Expr *e) { res = "[UNKNOWN EXPRESSION]"; }

    void ToStringVisitor::visit(TerminalExpr *e) {
        res = e->getAtom()->toString(*symbol_table);
    }

    void ToStringVisitor::visit(ParenExpr *e) {
        e->getBody()->accept(this);
        res = '(' + res + ')';
    }

    void ToStringVisitor::visit(CallExpr *e) {
        std::string expr_res = e->getCallee()->toString(*symbol_table) + '(';
        auto &args = e->getArguments();
        for (auto iter = args.begin(); iter != args.end(); iter++) {
            if (iter != args.begin())
                expr_res += ", ";
            iter->get()->accept(this);
            expr_res += res;
        }
        res = expr_res + ')';
    }

    void ToStringVisitor::visit(UnaryOpExpr *e) {
        const std::string op = toString(e->getOp());
        e->getValue()->accept(this);

        res = op + res;
    }

    void ToStringVisitor::visit(BinaryOpExpr *e) {
        const std::string op = toString(e->getOp());

        e->getLeft()->accept(this);
        const std::string left = res;

        e->getRight()->accept(this);
        const std::string right = res;

        res = left + ' ' + op + ' ' + right;
    }

} // namespace frontend::ast
