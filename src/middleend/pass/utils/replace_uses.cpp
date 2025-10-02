#include <memory>

#include "middleend/mir/mir.h"
#include "middleend/pass/utils/replace_uses.h"

namespace middleend {
    ReplaceUsesVisitor::ReplaceUsesVisitor(mir::Value *old_value,
                                           mir::Value *new_value,
                                           mir::BasicBlock *bb)
        : old_value(old_value), new_value(new_value), bb(bb) {
        new_value_literal = dynamic_cast<mir::Literal *>(new_value);
    }

    void ReplaceUsesVisitor::visit(mir::InstructionBinaryOp *i) {
        if (i->getLeft() == old_value)
            i->setLeft(addNewValue());
        if (i->getRight() == old_value)
            i->setRight(addNewValue());
    }

    mir::Value *ReplaceUsesVisitor::addNewValue() {
        if (!new_value_literal)
            return new_value;
        auto new_literal = std::make_unique<mir::Literal>(
            new_value_literal->getType(), new_value_literal->getValue());
        auto new_literal_ptr = new_literal.get();
        bb->getLiterals().push_back(std::move(new_literal));
        return new_literal_ptr;
    }

    void ReplaceUsesVisitor::visit(mir::InstructionCall *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionAlloca *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionLoad *i) {}

    void ReplaceUsesVisitor::visit(mir::InstructionStore *i) {
        if (i->getValue() == old_value)
            i->setValue(addNewValue());
    }

    void ReplaceUsesVisitor::visit(mir::InstructionPhi *i) {
        for (auto &pair : i->getPredecessors()) {
            if (pair.second == old_value)
                i->getPredecessors()[pair.first] = new_value;
        }
    }

    void ReplaceUsesVisitor::visit(mir::TerminatorReturn *t) {
        if (t->getValue() == old_value) {
            t->setValue(addNewValue());
        }
    }

    void ReplaceUsesVisitor::visit(mir::TerminatorBranch *t) {}

    void ReplaceUsesVisitor::visit(mir::TerminatorCondBranch *t) {
        if (t->getCond() == old_value)
            t->setCond(addNewValue());
    }
} // namespace middleend
