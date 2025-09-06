#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/type.h"

namespace middleend::mir {
    BasicBlock::BasicBlock(std::vector<Instruction *> body,
                           Terminator *terminator)
        : body(body), terminator(terminator) {}

    Function::Function(Type type, std::string name,
                       std::vector<BasicBlock> basic_blocks)
        : type(type), name(name), basic_blocks(basic_blocks) {}

    Program::Program(std::vector<Function> functions) : functions(functions) {}
} // namespace middleend::mir
