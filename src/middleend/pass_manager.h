#pragma once

#include "middleend/mir/mir.h"
#include "middleend/pass.h"

namespace middleend {
    class PassManager {
    public:
        void addPass(std::unique_ptr<TransformPass> p);
        void addPass(const std::string &pass_name);
        void runPasses(mir::Program &mir);

    private:
        std::vector<std::unique_ptr<TransformPass>> passes;
        std::vector<std::unique_ptr<AnalysisPass>> analyses;
    };

    PassManager initializeO0();
    PassManager initializeO1();
} // namespace middleend
