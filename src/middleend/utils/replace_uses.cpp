#include "middleend/utils/replace_uses.h"

namespace middleend {
    ReplaceUsesVisitor::ReplaceUsesVisitor(mir::Value *old_value,
                                           mir::Value *new_value)
        : old_value(old_value), new_value(new_value) {}

    void ReplaceUsesVisitor::visit(mir::InstructionBinaryOp *i) {
        if (i->getLeft() == old_value)
            i->setLeft(new_value);
        if (i->getRight() == old_value)
            i->setRight(new_value);
    }

    void ReplaceUsesVisitor::visit(mir::InstructionCall *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionAlloca *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionLoad *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionStore *i) {
        if (i->getValue() == old_value)
            i->setValue(new_value);
    }

    void ReplaceUsesVisitor::visit(mir::InstructionPhi *i) {
        for (auto &[bb, val] : i->getPredecessors()) {
            if (val == old_value)
                i->getPredecessors()[bb] = new_value;
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
