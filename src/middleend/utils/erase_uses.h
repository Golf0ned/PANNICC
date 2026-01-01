#pragma once

#include "middleend/mir/instruction.h"

namespace middleend {
    //
    // Erase visited instruction from the uses lists of its uses
    //
    class EraseUsesVisitor : public mir::InstructionVisitor {
    public:
        EraseUsesVisitor() = default;

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
    };
} // namespace middleend
