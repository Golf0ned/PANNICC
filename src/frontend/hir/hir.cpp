#include "hir.h"


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

    std::string Function::toString(std::unordered_map<uint64_t, std::string>& symbol_table) {
        std::string type_str = ::frontend::toString(type);
        std::string name_str = symbol_table.at(name->getValue());

        std::string res = type_str + " " + name_str + "()";
        // TODO: add body string

        return res;
    }


    std::string Program::toString() {
        return "HIR Program";
    }
}
