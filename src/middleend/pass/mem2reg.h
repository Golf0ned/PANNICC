#pragma once

#include "middleend/pass/pass.h"

namespace middleend {
    class Mem2Reg : TransformPass {
    public:
        void run(mir::Program &p) override;
    };
} // namespace middleend
