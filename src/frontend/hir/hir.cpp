#include "frontend/hir/hir.h"

namespace frontend::hir {
    Function::Function(Type type, std::unique_ptr<AtomIdentifier> name,
                       std::vector<Parameter> parameters,
                       std::vector<std::unique_ptr<Instruction>> body)
        : type(type), name(std::move(name)), parameters(std::move(parameters)),
          body(std::move(body)) {}

    Type Function::getType() { return type; }

    std::unique_ptr<AtomIdentifier> &Function::getName() { return name; }

    std::vector<Parameter> &Function::getParameters() { return parameters; }

    std::vector<std::unique_ptr<Instruction>> &Function::getBody() {
        return body;
    }

    std::string Function::toString(SymbolTable *symbol_table) {
        std::string type_str = ::frontend::toString(type);
        std::string name_str = name->toString(*symbol_table);

        std::string res = type_str + " " + name_str + "(";
        for (auto iter = parameters.begin(); iter != parameters.end(); iter++) {
            if (iter != parameters.begin())
                res += ", ";
            auto &[param_type, param_name] = *iter;
            res += ::frontend::toString(param_type) + ' ' +
                   param_name->toString(*symbol_table);
        }
        res += ") {\n";

        ToStringVisitor tsv(symbol_table);
        for (auto &i : body) {
            i->accept(&tsv);
            res += tsv.getResult() + '\n';
        }
        res += '}';

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
        : symbol_table(symbol_table), res("") {}

    std::string ToStringVisitor::getResult() { return res; }

    void ToStringVisitor::visit(Instruction *i) {
        res = "    [UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(Label *l) {
        const std::string label = l->getName()->toString(*symbol_table);
        res = "    " + label + ':';
    }

    void ToStringVisitor::visit(InstructionDeclaration *i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(*symbol_table);

        res = "    " + type + ' ' + variable + ';';
    }

    void ToStringVisitor::visit(InstructionAssignValue *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        const std::string value = i->getValue()->toString(*symbol_table);

        res = "    " + variable + " = " + value + ';';
    }

    void ToStringVisitor::visit(InstructionAssignUnaryOp *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        const std::string op = toString(i->getOp());
        const std::string value = i->getValue()->toString(*symbol_table);

        res = "    " + variable + " = " + op + value + ';';
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        const std::string op = toString(i->getOp());
        const std::string left = i->getLeft()->toString(*symbol_table);
        const std::string right = i->getRight()->toString(*symbol_table);

        res = "    " + variable + " = " + left + ' ' + op + ' ' + right + ';';
    }

    void ToStringVisitor::visit(InstructionReturn *i) {
        const std::string value = i->getValue()->toString(*symbol_table);

        res = "    return " + value + ';';
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        const std::string callee = i->getCallee()->toString(*symbol_table);
        res = "    " + callee + '(';
        auto &args = i->getArguments();
        for (auto iter = args.begin(); iter != args.end(); iter++) {
            if (iter != args.begin())
                res += ", ";
            res += iter->get()->toString(*symbol_table);
        }
        res += ");";
    }

    void ToStringVisitor::visit(InstructionCallAssign *i) {
        const std::string variable = i->getVariable()->toString(*symbol_table);
        const std::string callee = i->getCallee()->toString(*symbol_table);
        res = "    " + variable + " = " + callee + '(';
        auto &args = i->getArguments();
        for (auto iter = args.begin(); iter != args.end(); iter++) {
            if (iter != args.begin())
                res += ", ";
            res += iter->get()->toString(*symbol_table);
        }
        res += ");";
    }

    void ToStringVisitor::visit(InstructionBranch *i) {
        const std::string label = i->getLabel()->toString(*symbol_table);

        res = "    goto " + label + ';';
    }

    void ToStringVisitor::visit(InstructionBranchCond *i) {
        const std::string cond = i->getCmp()->toString(*symbol_table);
        const std::string t_label = i->getTLabel()->toString(*symbol_table);
        const std::string f_label = i->getFLabel()->toString(*symbol_table);

        res = "    goto_if (" + cond + ") " + t_label + ' ' + f_label + ';';
    }
} // namespace frontend::hir
