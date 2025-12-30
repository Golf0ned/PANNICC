#pragma once

#include "middleend/pass.h"

namespace middleend {
    class SplitCritical : public TransformPass {
    public:
        void run(mir::Program &p) override;
    };
} // namespace middleend
