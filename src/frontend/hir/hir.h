#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>


namespace frontend::hir {
    class Function {
        // TODO
    };

    class Program {
        public:
            std::string toString();
        private:
            std::vector<Function> functions;
            std::unordered_map<uint64_t, std::string> symbol_table;
    };

    // TODO: move to own file
    class Instruction;
}
