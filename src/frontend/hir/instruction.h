#pragma once

#include "frontend/ast/instruction.h"


namespace frontend::hir {
    class Instruction {
        virtual ~Instruction() = default;
    };
}
