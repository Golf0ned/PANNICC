#pragma once

#include "middleend/pass/dominator_tree.h"
#include "middleend/pass/pass.h"

namespace middleend {
    class Mem2Reg : public TransformPass {
    public:
        void run(mir::Program &p) override;
        void registerAnalyses(
            std::vector<std::unique_ptr<AnalysisPass>> &analyses) override;

    private:
        DominatorTree *dt;
    };

    class ReplaceUseVisitor : public mir::InstructionVisitor {
    public:
        ReplaceUseVisitor(mir::Value *old_value, mir::Value *new_value,
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
