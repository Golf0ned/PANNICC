#pragma once

#include <cstdint>
#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

namespace middleend::mir {
    class BasicBlock {
        private:
            uint64_t id;
            std::vector<Instruction> instructions;
            Terminator terminator;
    };

    class Function {
        private:
            Type type;
            uint64_t id;
            std::vector<BasicBlock> basic_blocks;
    };

    class Program {
        private:
            std::vector<Function> functions;
            // symbol table for function names
    };
} // namespace middleend::mir
