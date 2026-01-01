#pragma once

#include "middleend/mir/instruction.h"

namespace middleend {
    //
    // Replace all uses of old_value with new_value in visited instruction
    //
    class ReplaceUsesVisitor : public mir::InstructionVisitor {
    public:
        ReplaceUsesVisitor(mir::Value *old_value, mir::Value *new_value);

        void visit(mir::InstructionBinaryOp *i) override;
        void visit(mir::InstructionCall *i) override;
        void visit(mir::InstructionAlloca *i) override;
        void visit(mir::InstructionLoad *i) override;
        void visit(mir::InstructionStore *i) override;
        void visit(mir::InstructionPhi *i) override;
        void visit(mir::InstructionParallelCopy *i) override;

        void visit(mir::TerminatorReturn *t) override;
        void visit(mir::TerminatorBranch *t) override;
        void visit(mir::TerminatorCondBranch *t) override;

    private:
        mir::Value *old_value;
        mir::Value *new_value;
    };
} // namespace middleend
