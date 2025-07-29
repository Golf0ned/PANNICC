#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include "frontend/hir/atom.h"
#include "frontend/hir/instruction.h"


namespace frontend::hir {
    class Function {
        public:
            Function(Type type, AtomIdentifier* name, std::vector<Instruction*> body);
            Type getType();
            AtomIdentifier* getName();
            std::vector<Instruction*> getBody();
            std::string toString(std::unordered_map<uint64_t, std::string>& symbol_table);
        private:
            Type type;
            AtomIdentifier* name;
            // TODO: params
            std::vector<Instruction*> body;
    };

    class Program {
        public:
            std::string toString();
        private:
            std::vector<Function> functions;
            std::unordered_map<uint64_t, std::string> symbol_table;
    };

    // TODO: tostring
}
