#pragma once

#include "middleend/pass.h"

namespace middleend {
    class DominatorTree : public AnalysisPass {
    public:
        void run(mir::Program &p) override;
        bool dominates(mir::BasicBlock *bb1, mir::BasicBlock *bb2);
        mir::BasicBlock *getImmediateDominator(mir::BasicBlock *bb);
        const std::vector<mir::BasicBlock *> &getDominees(mir::BasicBlock *bb);

    private:
        std::unordered_map<mir::BasicBlock *, mir::BasicBlock *>
            immediate_dominators;
        std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock *>>
            immediate_dominated;
    };
} // namespace middleend
