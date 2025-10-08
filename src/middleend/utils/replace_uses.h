#pragma once

#include "middleend/mir/instruction.h"

namespace middleend {
    class ReplaceUsesVisitor : public mir::InstructionVisitor {
    public:
        ReplaceUsesVisitor(mir::Value *old_value, mir::Value *new_value);

        virtual void visit(mir::InstructionBinaryOp *i);
        virtual void visit(mir::InstructionCall *i);
        virtual void visit(mir::InstructionAlloca *i);
        virtual void visit(mir::InstructionLoad *i);
        virtual void visit(mir::InstructionStore *i);
        virtual void visit(mir::InstructionPhi *i);

        virtual void visit(mir::TerminatorReturn *t);
        virtual void visit(mir::TerminatorBranch *t);
        virtual void visit(mir::TerminatorCondBranch *t);

    private:
        mir::Value *old_value;
        mir::Value *new_value;
    };
} // namespace middleend
