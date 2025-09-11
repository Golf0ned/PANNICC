#include "middleend/pass/pass_manager.h"

namespace middleend {
    void PassManager::addPass(std::unique_ptr<TransformPass> p) {
        p->registerAnalyses(analyses);
        passes.push_back(std::move(p));
    }

    void PassManager::runPasses(mir::Program &mir) {
        for (auto &p : passes) {
            for (auto &a : p->getAnalyses())
                a->rerunIfInvalid(mir);
            p->run(mir);
        }
    }
} // namespace middleend
