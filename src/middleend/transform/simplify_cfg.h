#pragma once

#include "middleend/pass.h"

namespace middleend {
    class SimplifyCFG : public TransformPass {
    public:
        void run(mir::Program &p) override;
    };
} // namespace middleend
