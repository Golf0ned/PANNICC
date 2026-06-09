#pragma once

#include "middleend/pass.h"

namespace middleend {

class InstCombine : public TransformPass {
public:
    void run(mir::Program &p) override;
    void run(mir::Function *f);

    bool constantFold(mir::Instruction *i);
    bool simplifyArithmetic(mir::Instruction *i);
};

} // namespace middleend
