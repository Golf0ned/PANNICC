#include "middleend/pass/mem2reg.h"

namespace middleend {
    void Mem2Reg::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            // TODO: phi insertion
            // follow the algorithm, generate phis with store Value ptr

            // variable renaming
            // allocas are the initial variables

            // just look at pg 29 of ssa book man
            for (auto &bb : f.getBasicBlocks()) {
                for (auto &i : bb.getInstructions()) {
                }
            }
        }
    }
} // namespace middleend
