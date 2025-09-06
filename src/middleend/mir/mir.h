#pragma once

#include <vector>

#include "middleend/mir/type.h"

namespace middleend::mir {
    class Instruction;
    class Terminator;

    class BasicBlock {
    public:
        BasicBlock(std::vector<Instruction *> body, Terminator *terminator);

    private:
        std::vector<Instruction *> body;
        Terminator *terminator;
    };

    class Function {
    public:
        Function(Type type, std::string name,
                 std::vector<BasicBlock> basic_blocks);

    private:
        Type type;
        std::string name;
        std::vector<BasicBlock> basic_blocks;
    };

    class Program {
    public:
        Program(std::vector<Function> functions);

    private:
        std::vector<Function> functions;
    };
} // namespace middleend::mir
