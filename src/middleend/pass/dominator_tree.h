#pragma once

#include "middleend/pass/pass.h"

namespace middleend {
    class DominatorTree : public AnalysisPass {
    public:
        void run(mir::Program &p) override;

    private:
        std::unordered_map<mir::BasicBlock *, mir::BasicBlock *> predecessors;
        std::unordered_map<mir::BasicBlock *, std::vector<mir::BasicBlock>>
            successors;
    };
} // namespace middleend
