#include "middleend/utils/erase_uses.h"

namespace middleend {
    void EraseUsesVisitor::visit(mir::InstructionBinaryOp *i) {
        auto i_ptr = static_cast<mir::Value *>(i);
        i->delUse(i->getLeft());
        i->delUse(i->getRight());
    }

    void EraseUsesVisitor::visit(mir::InstructionCall *i) {}

    void EraseUsesVisitor::visit(mir::InstructionAlloca *i) {}

    void EraseUsesVisitor::visit(mir::InstructionLoad *i) {
        i->delUse(i->getPtr());
    }

    void EraseUsesVisitor::visit(mir::InstructionStore *i) {
        i->delUse(i->getPtr());
        i->delUse(i->getValue());
    }

    void EraseUsesVisitor::visit(mir::InstructionPhi *i) {
        // for (auto [_, val] : i->getPredecessors())
        //     i->delUse(val);
    }

    void EraseUsesVisitor::visit(mir::TerminatorReturn *t) {
        t->delUse(t->getValue());
    }

    void EraseUsesVisitor::visit(mir::TerminatorBranch *t) {}
    void EraseUsesVisitor::visit(mir::TerminatorCondBranch *t) {
        t->delUse(t->getCond());
    }
} // namespace middleend
