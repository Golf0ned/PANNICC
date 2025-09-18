#include <deque>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/pass/mem2reg.h"

namespace middleend {
    void Mem2Reg::run(mir::Program &p) {
        for (auto &f : p.getFunctions()) {
            // TODO: phi insertion
            // follow the algorithm, insert phis

            //
            // Variable renaming
            //
            std::vector<std::unique_ptr<mir::Instruction>> toPromote;
            std::unordered_map<mir::InstructionAlloca *, mir::Value *> curVal;

            std::unordered_set<mir::BasicBlock *> visited;
            std::deque<mir::BasicBlock *> worklist;
            worklist.push_back(&f.getBasicBlocks()[0]);
            bool isEntry = true;

            while (!worklist.empty()) {
                auto cur = worklist.back();
                worklist.pop_back();

                // Visited before: only update phis
                if (visited.contains(cur)) {
                    // TODO: update phis
                    continue;
                }

                visited.insert(cur);

                // Not visited: update phis and rename in instructions
                auto &instructions = cur->getInstructions();
                for (int ind = 0, end = instructions.size(); ind < end; ind++) {
                    auto &i = instructions[ind];

                    // phi: add value from previous basic block
                    // TODO: update phis

                    // alloca: if promotable (lives in entry block),
                    // steal from instructions
                    auto *alloca =
                        dynamic_cast<mir::InstructionAlloca *>(i.get());
                    if (isEntry && alloca) {
                        toPromote.push_back(std::move(i));
                        instructions.erase(instructions.begin() + ind);
                        ind--;
                        end--;
                        continue;
                    }

                    // store: update current value mapping
                    // remove store from instructions
                    auto *store =
                        dynamic_cast<mir::InstructionStore *>(i.get());
                    if (store) {
                        curVal[store->getPtr()] = store->getValue();
                        instructions.erase(instructions.begin() + ind);
                        ind--;
                        end--;
                        continue;
                    }

                    // load: replace all uses of the load with current value
                    // remove load from instructions
                    auto *load = dynamic_cast<mir::InstructionLoad *>(i.get());
                    if (load) {
                        ReplaceUseVisitor visitor(load, curVal[load->getPtr()]);
                        for (auto &use : load->getUses()) {
                            use->accept(&visitor);
                        }
                        instructions.erase(instructions.begin() + ind);
                        ind--;
                        end--;
                        continue;
                    }
                }

                for (auto &next : cur->getDescendants()) {
                    worklist.push_back(next);
                }

                isEntry = false;
            }
        }
    }

    ReplaceUseVisitor::ReplaceUseVisitor(mir::Value *oldValue,
                                         mir::Value *newValue)
        : oldValue(oldValue), newValue(newValue) {}

    void ReplaceUseVisitor::visit(mir::InstructionBinaryOp *i) {
        if (i->getLeft() == oldValue)
            i->setLeft(newValue);
        if (i->getRight() == oldValue)
            i->setRight(newValue);
    }

    void ReplaceUseVisitor::visit(mir::InstructionCall *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionAlloca *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionLoad *i) {}

    void ReplaceUseVisitor::visit(mir::InstructionStore *i) {
        if (i->getValue() == oldValue)
            i->setValue(newValue);
    }

    void ReplaceUseVisitor::visit(mir::InstructionPhi *i) {}

    void ReplaceUseVisitor::visit(mir::TerminatorReturn *t) {
        if (t->getValue() == oldValue) {
            t->setValue(newValue);
        }
    }

    void ReplaceUseVisitor::visit(mir::TerminatorBranch *t) {}

    void ReplaceUseVisitor::visit(mir::TerminatorCondBranch *t) {}
} // namespace middleend
