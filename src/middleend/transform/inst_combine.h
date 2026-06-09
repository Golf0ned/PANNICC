#pragma once

#include "middleend/pass.h"

namespace middleend {

class InstCombine : public TransformPass {
public:
    void run(mir::Program &p) override;
    void run(mir::Function *f);
};

} // namespace middleend
