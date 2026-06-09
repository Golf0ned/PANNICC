#include "middleend/transform/gvn.h"

namespace middleend {
void GVN::run(mir::Program &p) {
    for (auto &f : p.getFunctions())
        run(f.get());
}

void GVN::run(mir::Function *f) {
    auto definition = dynamic_cast<mir::FunctionDefinition *>(f);
    if (!definition)
        return;
}

void GVN::registerAnalyses(
    std::vector<std::unique_ptr<AnalysisPass>> &analyses) {
    for (auto &pass : analyses) {
        auto dt = dynamic_cast<DominatorTree *>(pass.get());
        if (dt) {
            this->dt = dt;
            required_analyses.push_back(dt);
            continue;
        }
    }

    if (!this->dt) {
        auto dt = std::make_unique<DominatorTree>();
        this->dt = dt.get();
        required_analyses.push_back(dt.get());
        analyses.push_back(std::move(dt));
    }
}
} // namespace middleend
