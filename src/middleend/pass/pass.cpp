#include "middleend/pass/pass.h"

namespace middleend {
    void AnalysisPass::invalidate() { valid = false; }

    void AnalysisPass::rerunIfInvalid(mir::Program &p) {
        if (!valid)
            run(p);
    }

    void TransformPass::registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) {}

    const std::vector<AnalysisPass *> &TransformPass::getAnalyses() {
        return requiredAnalyses;
    }
} // namespace middleend
