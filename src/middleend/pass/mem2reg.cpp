#include <deque>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/pass/mem2reg.h"

namespace middleend {
    void Mem2Reg::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            // TODO: phi insertion
            // follow the algorithm, generate phis with store Value ptr

            // variable renaming
            // allocas are the initial variables

            // just look at pg 29 of ssa book man
            std::unordered_set<mir::BasicBlock *> visited;
            std::deque<mir::BasicBlock *> worklist;
            std::unordered_map<mir::InstructionAlloca *, mir::Value *> curVal;
            worklist.push_back(&f.getBasicBlocks()[0]);
            while (!worklist.empty()) {
                auto cur = worklist.back();
                worklist.pop_back();

                if (visited.contains(cur)) {
                    // TODO: update phis
                    continue;
                }

                for (auto &i : cur->getInstructions()) {
                    // auto *alloca =
                    //     dynamic_cast<mir::InstructionAlloca *>(i.get());
                    // if (alloca)
                    //     curVal[alloca] = nullptr;
                    auto *load = dynamic_cast<mir::InstructionLoad *>(i.get());
                    if (load) {
                        // updateReachingDefinition(alloca, i)
                        // replace use of alloca in next instruction with i;
                    }
                    auto *store =
                        dynamic_cast<mir::InstructionStore *>(i.get());
                    if (store) {
                        // updateReachingDefinition(alloca, i)
                        // create fresh variable v???? (done)
                        // replace defn of alloca w/ v in i (done)
                        // reaching definition[v] = reaching definition[alloca]
                        // reaching definition[alloca] = v
                    }
                }

                for (auto &next : cur->getDescendants()) {
                    worklist.push_back(next);
                }
            }
        }
    }
} // namespace middleend
