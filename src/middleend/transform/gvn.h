#pragma once

#include "middleend/pass.h"

namespace middleend {

class GVN : public TransformPass {
public:
    void run(mir::Program &p) override;
    void registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;
};

} // namespace middleend
