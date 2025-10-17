#pragma once

#include "middleend/analysis/dominator_tree.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/pass.h"

namespace middleend {
    class Mem2Reg : public TransformPass {
    public:
        void run(mir::Program &p) override;
        void registerAnalyses(
            std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

    private:
        DominatorTree *dt;
        NumberIR *nir;
    };
} // namespace middleend
