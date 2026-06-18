#pragma once

#include "middleend/analysis/dominator_tree.h"
#include "middleend/pass.h"

namespace middleend {

// TODO: make gvn hash

class GVN : public TransformPass {
public:
    GVN();
    void run(mir::Program &p) override;
    void run(mir::Function *f);

    void dvnt(mir::BasicBlock *bb);

    void registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

private:
    DominatorTree *dt;
    uint64_t next_num;
    // TODO: figure out hash hell
    std::unordered_map<uint64_t, mir::Value *> num_to_value;
};

} // namespace middleend
