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

    uint64_t NumberIR::getNumber(mir::BasicBlock *bb) {
        return basic_block_ids.at(bb);
    }

    uint64_t NumberIR::getNumber(mir::Value *i) {
        if (instruction_ids.contains(i))
            return instruction_ids.at(i);
        // If not found, -1
        // that's always going to be mapped to a basic block so who cares
        return -1;
    }
} // namespace middleend
