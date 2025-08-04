#include "frontend/hir/hir.h"


namespace frontend::hir {
    Function::Function(Type type, AtomIdentifier* name, std::vector<Instruction*> body)
        : type(type), name(name), body(body) {}

    Type Function::getType() {
        return type;
    }

    AtomIdentifier* Function::getName() {
        return name;
    }

    std::vector<Instruction*> Function::getBody() {
        return body;
    }

    std::string Function::toString(SymbolTable& symbol_table) {
        std::string type_str = ::frontend::toString(type);
        std::string name_str = name->toString(symbol_table);

        std::string res = type_str + " " + name_str + "() {\n";
        ToStringVisitor visitor = ToStringVisitor(symbol_table);
        for (Instruction* i : body) {
            i->accept(&visitor);
            res += visitor.getResult() + '\n';
        }
        res += '}';

        return res;
    }


    Program::Program(std::vector<Function> functions, SymbolTable& symbol_table)
        : functions(functions), symbol_table(symbol_table) {}

    std::vector<Function>& Program::getFunctions() {
        return functions;
    }

    SymbolTable& Program::getSymbolTable() {
        return symbol_table;
    }

    std::string Program::toString() {
        std::string res = "";

        for (Function f : functions) {
            res += f.toString(symbol_table) + "\n";
        }

        return res;
    }

    Program::~Program() {
        for (Function& f : functions) {
            for (auto i : f.getBody()) {
                delete i;
            }
            delete f.getName();
        }
    }


    ToStringVisitor::ToStringVisitor(SymbolTable& symbol_table)
        : symbol_table(symbol_table), res("") {}

    std::string ToStringVisitor::getResult() {
        return res;
    }

    void ToStringVisitor::visit(Instruction* i) {
        res = "    [UNKNOWN INSTRUCTION]";
    }

    void ToStringVisitor::visit(InstructionDeclaration* i) {
        const std::string type = toString(i->getType());
        const std::string variable = i->getVariable()->toString(symbol_table);

        res = "    DECLARE " + type + " " + variable;
    }

    void ToStringVisitor::visit(InstructionAssignValue* i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string value = i->getValue()->toString(symbol_table);

        res = "    ASSIGN " + variable + " = " + value;
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp* i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string op = toString(i->getOp());
        const std::string left = i->getLeft()->toString(symbol_table);
        const std::string right = i->getRight()->toString(symbol_table);

        res = "    ASSIGN " + variable + " = " + left + " " + op + " " + right;
    }

    void ToStringVisitor::visit(InstructionReturn* i) {
        const std::string value = i->getValue()->toString(symbol_table);

        res = "    RETURN " + value;
    }

    void ToStringVisitor::visit(InstructionCall* i) {
        const std::string target = i->getTarget()->toString(symbol_table);

        res = "    CALL " + target;
    }

    void ToStringVisitor::visit(InstructionCallAssign* i) {
        const std::string variable = i->getVariable()->toString(symbol_table);
        const std::string target = i->getTarget()->toString(symbol_table);

        res = "    ASSIGN " + variable + " = CALL " + target;
    }
}
