#include "backend/lir/lir.h"

namespace backend::lir {
    Block::Block(std::string label,
                 std::list<std::unique_ptr<Instruction>> body)
        : label(label), body(std::move(body)) {}

    std::string Block::getLabel() { return label; }

    std::list<std::unique_ptr<Instruction>> &Block::getBody() { return body; }

    Program::Program(std::list<std::unique_ptr<Block>> blocks)
        : blocks(std::move(blocks)) {}

    std::list<std::unique_ptr<Block>> &Program::getBlocks() { return blocks; }
} // namespace backend::lir
