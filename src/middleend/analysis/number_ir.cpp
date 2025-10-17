#include "middleend/analysis/number_ir.h"

namespace middleend {
    void NumberIR::run(mir::Program &p) {
        uint64_t counter = 0;

        for (auto &f : p.getFunctions()) {
            auto entry = f->getEntryBlock();
            for (auto &bb : f->getBasicBlocks()) {
                basic_block_ids[bb.get()] = bb.get() == entry ? -1 : counter++;

                for (auto &i : bb->getInstructions()) {
                    auto v = dynamic_cast<mir::Value *>(i.get());
                    if (v)
                        instruction_ids[v] = counter++;
                }

                auto t = dynamic_cast<mir::Value *>(bb->getTerminator().get());
                if (t)
                    instruction_ids[t] = counter++;
            }
        }
    }

    int NumberIR::bbCmp(mir::BasicBlock *first, mir::BasicBlock *second) {
        uint64_t first_val = basic_block_ids.at(first);
        uint64_t second_val = basic_block_ids.at(second);

        if (first_val > second_val)
            return 1;
        if (first_val < second_val)
            return -1;
        return 0;
    }

    int NumberIR::instCmp(mir::Value *first, mir::Value *second) {
        uint64_t first_val = instruction_ids.at(first);
        uint64_t second_val = instruction_ids.at(second);

        if (first_val > second_val)
            return 1;
        if (first_val < second_val)
            return -1;
        return 0;
    }
} // namespace middleend
