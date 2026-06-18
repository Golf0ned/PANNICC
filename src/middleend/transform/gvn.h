#pragma once

#include "middleend/analysis/dominator_tree.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/pass.h"

namespace middleend {

// TODO: make gvn hash

class GVN : public TransformPass {
public:
    GVN();
    void run(mir::Program &p) override;
    void run(mir::Function *f);

    void dvnt(mir::BasicBlock *bb);
    std::string gvnHash(mir::Value *v);

    void registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

private:
    DominatorTree *dt;
    NumberIR *nir;
    uint64_t next_num;
    std::unordered_map<std::string, uint64_t> value_to_num;
    std::unordered_map<uint64_t, mir::Value *> num_to_value;
};

} // namespace middleend
