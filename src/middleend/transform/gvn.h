#pragma once

#include "middleend/analysis/dominator_tree.h"
#include "middleend/pass.h"

namespace middleend {

class HashGVN : public TransformPass {
public:
    void run(mir::Program &p) override;
    void run(mir::Function *f);

    void dvnt(mir::BasicBlock *bb);
    std::string gvnHash(mir::Value *v);

    void registerAnalyses(
        std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

private:
    DominatorTree *dt;
    std::unordered_map<std::string, mir::Value *> value_numberings;
};

} // namespace middleend
