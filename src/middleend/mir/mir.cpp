#include <cstdint>
#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/type.h"

namespace middleend::mir {
    BasicBlock::BasicBlock(uint64_t id, std::vector<Instruction> body,
                           Terminator terminator)
        : id(id), body(body), terminator(terminator) {}

    Function::Function(Type type, uint64_t id,
                       std::vector<BasicBlock> basic_blocks)
        : type(type), id(id), basic_blocks(basic_blocks) {}

    Program::Program(std::vector<Function> functions) : functions(functions) {}
} // namespace middleend::mir
