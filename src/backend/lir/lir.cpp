#include "backend/lir/lir.h"

namespace backend::lir {
    Program::Program(std::list<std::unique_ptr<Instruction>> instructions)
        : instructions(std::move(instructions)) {}

    std::list<std::unique_ptr<Instruction>> &Program::getInstructions() {
        return instructions;
    }

    std::string Program::toString() { return ""; }
} // namespace backend::lir
