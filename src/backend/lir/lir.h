#pragma once

#include <list>
#include <memory>

#include "backend/lir/instruction.h"

namespace backend::lir {
    class Program {
    public:
        Program(std::list<std::unique_ptr<Instruction>> instructions);
        std::list<std::unique_ptr<Instruction>> &getInstructions();
        std::string toString();

    private:
        std::list<std::unique_ptr<Instruction>> instructions;
        OperandManager om;
    };
} // namespace backend::lir
