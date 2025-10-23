#pragma once

#include <list>
#include <memory>

#include "backend/lir/instruction.h"

namespace backend::lir {
    class Block {
    public:
        Block(std::string label, std::list<std::unique_ptr<Instruction>> body);
        std::string getLabel();
        std::list<std::unique_ptr<Instruction>> &getBody();

    private:
        std::string label;
        std::list<std::unique_ptr<Instruction>> body;
    };

    class Program {
    public:
        Program(std::list<std::unique_ptr<Block>> blocks);
        std::list<std::unique_ptr<Block>> &getBlocks();

    private:
        std::list<std::unique_ptr<Block>> blocks;
    };
} // namespace backend::lir
