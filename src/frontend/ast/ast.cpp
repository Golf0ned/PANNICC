#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

#include <string>

using namespace frontend;
using namespace frontend::ast;

FunctionDefinition::FunctionDefinition(std::unique_ptr<Type> type,
                                       std::unique_ptr<AtomIdentifier> name,
                                       std::vector<Parameter> parameters,
                                       std::unique_ptr<Scope> body)
    : type(std::move(type)), name(std::move(name)),
      parameters(std::move(parameters)), body(std::move(body)) {}

Type *FunctionDefinition::getType() { return type.get(); }

AtomIdentifier *FunctionDefinition::getName() { return name.get(); }

std::vector<Parameter> &FunctionDefinition::getParameters() {
    return parameters;
}

Scope *FunctionDefinition::getBody() { return body.get(); }

std::string FunctionDefinition::toString(SymbolTable &st) {
    std::string type_str = type->toString();
    std::string name_str = name->toString(st);

    std::string res = type_str + " " + name_str + '(';
    for (auto iter = parameters.begin(); iter != parameters.end(); iter++) {
        if (iter != parameters.begin())
            res += ", ";
        auto &[param_type, param_name] = *iter;
        res += param_type->toString() + ' ' + param_name->toString(st);
    }
    res += ')';

    ToStringVisitor tsv(st);
    body->accept(&tsv);
    res += " " + tsv.getResult();

    return res;
}

void FunctionDefinition::accept(FunctionVisitor *v) { v->visit(this); }

FunctionPrototype::FunctionPrototype(std::unique_ptr<Type> type,
                                     std::unique_ptr<AtomIdentifier> name,
                                     std::vector<Parameter> parameters)
    : type(std::move(type)), name(std::move(name)),
      parameters(std::move(parameters)) {}

Type *FunctionPrototype::getType() { return type.get(); }

AtomIdentifier *FunctionPrototype::getName() { return name.get(); }

std::vector<Parameter> &FunctionPrototype::getParameters() {
    return parameters;
}

std::string FunctionPrototype::toString(SymbolTable &st) {
    std::string type_str = type->toString();
    std::string name_str = name->toString(st);

    std::string res = type_str + " " + name_str + '(';
    for (auto iter = parameters.begin(); iter != parameters.end(); iter++) {
        if (iter != parameters.begin())
            res += ", ";
        auto &[param_type, param_name] = *iter;
        res += param_type->toString() + ' ' + param_name->toString(st);
    }
    res += ");";

    return res;
}

void FunctionPrototype::accept(FunctionVisitor *v) { v->visit(this); }

Program::Program(std::vector<std::unique_ptr<Function>> functions,
                 SymbolTable st)
    : functions(std::move(functions)), st(std::move(st)) {}

std::vector<std::unique_ptr<Function>> &Program::getFunctions() {
    return functions;
}

SymbolTable &Program::getSymbolTable() { return st; }

std::string Program::toString() {
    std::string res = "";

    for (auto iter = functions.begin(); iter != functions.end(); iter++) {
        if (iter != functions.begin())
            res += "\n\n";
        res += (*iter)->toString(st);
    }

    return res;
}

ToStringVisitor::ToStringVisitor(SymbolTable &st)
    : st(st), prefix(""), res("") {}

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
    const std::string type = i->getType()->toString();
    const std::string variable = i->getVariable()->toString(st);

    std::string declare_res = prefix + type;
    if (!type.ends_with('*'))
        declare_res += " ";
    declare_res += variable;

    auto value = i->getValue();
    if (value) {
        value->accept(this);
        declare_res += " = " + res;
    }

    res = declare_res + ";";
}

void ToStringVisitor::visit(InstructionReturn *i) {
    i->getValue()->accept(this);
    res = prefix + "return " + res + ";";
}

void ToStringVisitor::visit(InstructionIf *i) {
    i->getCond()->accept(this);
    const std::string cond = res;
    std::string if_res = prefix + "if (" + cond + ")";

    convertSubexpression(i->getTBranch());
    std::string t_branch = getResult();
    if_res += t_branch;

    if (i->hasFBranch()) {
        convertSubexpression(i->getFBranch());
        std::string f_branch = getResult();
        if_res += dynamic_cast<Scope *>(i->getTBranch()) != nullptr
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

    convertSubexpression(i->getBody());
    std::string body = getResult();
    while_res += body;

    res = while_res;
}

void ToStringVisitor::visit(Expr *e) { res = "[UNKNOWN EXPRESSION]"; }

void ToStringVisitor::visit(TerminalExpr *e) {
    res = e->getAtom()->toString(st);
}

void ToStringVisitor::visit(ParenExpr *e) {
    e->getBody()->accept(this);
    res = '(' + res + ')';
}

void ToStringVisitor::visit(CallExpr *e) {
    std::string expr_res = e->getCallee()->toString(st) + '(';
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
