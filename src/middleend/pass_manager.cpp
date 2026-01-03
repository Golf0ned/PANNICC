#include <stdexcept>

#include "middleend/pass_manager.h"
#include "middleend/transform/insert_parallel_copies.h"
#include "middleend/transform/inst_combine.h"
#include "middleend/transform/mem2reg.h"
#include "middleend/transform/simplify_cfg.h"

namespace middleend {
    void PassManager::addPass(std::unique_ptr<TransformPass> p) {
        p->registerAnalyses(analyses);
        passes.push_back(std::move(p));
    }

    void PassManager::addPass(const std::string &pass_name) {
        if (pass_name == "insert_parallel_copies")
            addPass(std::make_unique<middleend::InsertParallelCopies>());
        else if (pass_name == "inst_combine")
            addPass(std::make_unique<middleend::InstCombine>());
        else if (pass_name == "mem2reg")
            addPass(std::make_unique<middleend::Mem2Reg>());
        else if (pass_name == "simplify_cfg")
            addPass(std::make_unique<middleend::SimplifyCFG>());
        else if (pass_name == "split_critical")
            addPass(std::make_unique<middleend::InstCombine>());
        else
            throw std::invalid_argument("invalid pass: \"" + pass_name + "\"");
    }

    void PassManager::runPasses(mir::Program &mir) {
        for (auto &p : passes) {
            for (auto &a : p->getAnalyses())
                a->rerunIfInvalid(mir);
            p->run(mir);
        }
    }

    std::unique_ptr<PassManager> initializeO0() {
        return std::make_unique<PassManager>();
    }

    std::unique_ptr<PassManager> initializeO1() {
        auto pm = std::make_unique<PassManager>();
        pm->addPass(std::make_unique<middleend::Mem2Reg>());
        pm->addPass(std::make_unique<middleend::InstCombine>());
        pm->addPass(std::make_unique<middleend::SimplifyCFG>());
        return pm;
    }
} // namespace middleend
