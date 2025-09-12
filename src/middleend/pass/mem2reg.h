#pragma once

#include "middleend/pass/pass.h"

namespace middleend {
    class Mem2Reg : public TransformPass {
    public:
        void run(mir::Program &p) override;
    };

    class ReplaceUseVisitor : public mir::InstructionVisitor {
    public:
        ReplaceUseVisitor(mir::Value *oldValue, mir::Value *newValue);

        void visit(mir::InstructionBinaryOp *i);
        void visit(mir::InstructionCall *i);
        void visit(mir::InstructionAlloca *i);
        void visit(mir::InstructionLoad *i);
        void visit(mir::InstructionStore *i);

        void visit(mir::TerminatorReturn *t);

    private:
        mir::Value *oldValue;
        mir::Value *newValue;
    };
} // namespace middleend
