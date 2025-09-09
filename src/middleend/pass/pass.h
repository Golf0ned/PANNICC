#pragma once

#include "middleend/mir/mir.h"

namespace middleend {
    class Pass {
    public:
        virtual void run(mir::Program &p) = 0;
        // virtual void run(mir::Function &f) = 0;
        // virtual void run(mir::BasicBlock &b) = 0;
    };

    class AnalysisPass : public Pass {
    public:
        void invalidate();
        void rerunIfInvalid();
        // type getResult();

    private:
        bool valid;
        // type result;
    };

    class TransformPass : public Pass {
    public:
        virtual void
        registerAnalyses(std::vector<std::unique_ptr<AnalysisPass>> &analyses);
        const std::vector<AnalysisPass *> &getAnalyses();

    private:
        std::vector<AnalysisPass *> requiredAnalyses;
    };
} // namespace middleend
