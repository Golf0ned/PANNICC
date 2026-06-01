#include "middleend/utils/replace_uses.h"

#include <ranges>
#include <utility>

namespace middleend {

void replaceUses(mir::Value *old_value, mir::Value *new_value) {
    ReplaceUsesVisitor ruv(old_value, new_value);
    auto uses_range = std::views::keys(old_value->getUses());
    std::vector<mir::Instruction *> uses(uses_range.begin(), uses_range.end());
    for (auto &use : uses)
        use->accept(&ruv);
}

ReplaceUsesVisitor::ReplaceUsesVisitor(mir::Value *old_value,
                                       mir::Value *new_value)
    : old_value(old_value), new_value(new_value) {}

void ReplaceUsesVisitor::visit(mir::InstructionBinaryOp *i) {
    if (i->getLeft() == old_value)
        i->setLeft(new_value);
    if (i->getRight() == old_value)
        i->setRight(new_value);
}

void ReplaceUsesVisitor::visit(mir::InstructionCall *i) {
    auto &args = i->getArguments();
    for (auto iter = args.begin(); iter != args.end(); iter++) {
        auto *val = *iter;
        if (val == old_value) {
            i->delUse(val);
            i->addUse(new_value);
            *iter = new_value;
        }
    }
}

void ReplaceUsesVisitor::visit(mir::InstructionAlloca *i) {}

void ReplaceUsesVisitor::visit(mir::InstructionLoad *i) {
    if (i->getPtr() == old_value)
        i->setPtr(new_value);
}

void ReplaceUsesVisitor::visit(mir::InstructionStore *i) {
    if (i->getValue() == old_value)
        i->setValue(new_value);

    if (i->getPtr() == old_value)
        i->setPtr(new_value);
}

void ReplaceUsesVisitor::visit(mir::InstructionPhi *i) {
    for (auto &[bb, val] : i->getPredecessors()) {
        if (val == old_value)
            i->setPredecessor(bb, new_value);
    }
}

void ReplaceUsesVisitor::visit(mir::InstructionParallelCopy *i) {
    for (auto &[phi_val, copy_val] : i->getCopies()) {
        if (phi_val == old_value) {
            // TODO: do this in a way that doesnt invalidate the iterator
            std::unreachable();
        }
        if (copy_val == old_value)
            i->setCopy(phi_val, new_value);
    }
}

void ReplaceUsesVisitor::visit(mir::TerminatorReturn *t) {
    if (t->getValue() == old_value) {
        t->setValue(new_value);
    }
}

void ReplaceUsesVisitor::visit(mir::TerminatorBranch *t) {}

void ReplaceUsesVisitor::visit(mir::TerminatorCondBranch *t) {
    if (t->getCond() == old_value)
        t->setCond(new_value);
}

} // namespace middleend
