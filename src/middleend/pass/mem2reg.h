#pragma once

#include "middleend/pass/dominator_tree.h"
#include "middleend/pass/pass.h"

namespace middleend {
    class Mem2Reg : public TransformPass {
    public:
        void run(mir::Program &p) override;
        void registerAnalyses(
            std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

    private:
        DominatorTree *dt;
    };
} // namespace middleend
