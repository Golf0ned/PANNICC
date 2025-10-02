#pragma once

#include "middleend/mir/instruction.h"

namespace middleend {
    class ReplaceUsesVisitor : public mir::InstructionVisitor {
    public:
        ReplaceUsesVisitor(mir::Value *old_value, mir::Value *new_value,
                           mir::BasicBlock *bb);

        mir::Value *addNewValue();

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
        mir::Literal *new_value_literal;
        mir::BasicBlock *bb;
    };
} // namespace middleend
