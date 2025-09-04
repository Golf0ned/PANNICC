#pragma once

#include <cstdint>
#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

namespace middleend::mir {
    class BasicBlock {
    public:
        BasicBlock(uint64_t id, std::vector<Instruction> body,
                   Terminator terminator);

    private:
        uint64_t id;
        std::vector<Instruction> body;
        Terminator terminator;
    };

    class Function {
    public:
        Function(Type type, uint64_t id, std::vector<BasicBlock> basic_blocks);

    private:
        Type type;
        uint64_t id;
        std::vector<BasicBlock> basic_blocks;
    };

    class Program {
        Program(std::vector<Function> functions);

    private:
        std::vector<Function> functions;
        // symbol table for function names
    };
} // namespace middleend::mir
