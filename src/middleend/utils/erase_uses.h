#pragma once

#include "middleend/mir/instruction.h"

namespace middleend {
    //
    // Erase visited instruction from the uses lists of its uses
    //
    class EraseUsesVisitor : public mir::InstructionVisitor {
    public:
        EraseUsesVisitor() = default;

        virtual void visit(mir::InstructionBinaryOp *i);
        virtual void visit(mir::InstructionCall *i);
        virtual void visit(mir::InstructionAlloca *i);
        virtual void visit(mir::InstructionLoad *i);
        virtual void visit(mir::InstructionStore *i);
        virtual void visit(mir::InstructionPhi *i);

        virtual void visit(mir::TerminatorReturn *t);
        virtual void visit(mir::TerminatorBranch *t);
        virtual void visit(mir::TerminatorCondBranch *t);
    };
} // namespace middleend
