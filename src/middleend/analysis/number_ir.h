#pragma once

#include "middleend/pass.h"

namespace middleend {
    class NumberIR : public AnalysisPass {
    public:
        void run(mir::Program &p) override;
        uint64_t getNumber(mir::BasicBlock *bb);
        uint64_t getNumber(mir::Value *i);

    private:
        std::unordered_map<mir::BasicBlock *, uint64_t> basic_block_ids;
        std::unordered_map<mir::Value *, uint64_t> instruction_ids;
    };
} // namespace middleend
