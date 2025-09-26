#include <algorithm>

#include "middleend/pass/simplify_cfg.h"

namespace middleend {
    void SimplifyCFG::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            bool changed = true;
            while (changed) {
                changed = false;
                std::vector<size_t> to_erase;
                auto &bbs = f.getBasicBlocks();
                for (size_t ind = 0; ind < bbs.size(); ind++) {
                    auto &bb = bbs[ind];

                    // Remove if no predecessors
                    if (bb->getPredecessors().empty() &&
                        bb.get() != f.getEntryBlock()) {
                        for (auto pred : bb->getPredecessors()) {
                            auto &succs = pred->getSuccessors();
                            succs.erase(std::find(succs.begin(), succs.end(),
                                                  bb.get()));
                        }

                        for (auto succ : bb->getSuccessors()) {
                            auto &preds = succ->getPredecessors();
                            preds.erase(std::find(preds.begin(), preds.end(),
                                                  bb.get()));
                        }

                        to_erase.push_back(ind);

                        changed = true;
                        continue;
                    }
                    // Merge if straight line connection
                    // Eliminate phis if only one successor
                    // Eliminate if only unconditional br
                }

                //
                // Commit changes
                //
                if (to_erase.size() == 0)
                    continue;

                std::vector<std::unique_ptr<mir::BasicBlock>> new_bbs;
                new_bbs.reserve(bbs.size() - to_erase.size());

                size_t to_erase_ind = 0;
                for (size_t ind = 0; ind < bbs.size(); ind++) {
                    auto &bb = bbs[ind];
                    if (to_erase_ind < to_erase.size() &&
                        ind == to_erase[to_erase_ind]) {
                        to_erase_ind++;
                        continue;
                    }
                    new_bbs.push_back(std::move(bb));
                }

                f.getBasicBlocks().swap(new_bbs);
            }
        }
    }
} // namespace middleend
