#include "middleend/pass_manager.h"
#include "middleend/transform/inst_combine.h"
#include "middleend/transform/mem2reg.h"
#include "middleend/transform/simplify_cfg.h"

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

    PassManager initializeO0() {
        PassManager pm;
        return pm;
    }

    PassManager initializeO1() {
        PassManager pm;
        pm.addPass(std::make_unique<middleend::Mem2Reg>());
        pm.addPass(std::make_unique<middleend::InstCombine>());
        pm.addPass(std::make_unique<middleend::SimplifyCFG>());
        return pm;
    }
} // namespace middleend
