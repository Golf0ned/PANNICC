#pragma once

#include "middleend/pass/pass.h"

namespace middleend {
    class InstCombine : public TransformPass {
    public:
        void run(mir::Program &p) override;
    };
} // namespace middleend
