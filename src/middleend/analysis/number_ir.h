#pragma once

#include "middleend/pass.h"

namespace middleend {
    class NumberIR : public AnalysisPass {
    public:
        void run(mir::Program &p) override;
        int bbCmp(mir::BasicBlock *first, mir::BasicBlock *second);
        int instCmp(mir::Value *first, mir::Value *second);

    private:
        std::unordered_map<mir::BasicBlock *, uint64_t> basic_block_ids;
        std::unordered_map<mir::Value *, uint64_t> instruction_ids;
    };
} // namespace middleend
