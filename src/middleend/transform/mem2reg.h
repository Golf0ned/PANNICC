#pragma once

#include "middleend/analysis/dominator_tree.h"
#include "middleend/pass.h"

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
                          mir::Program &p);

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
        mir::Program &p;
    };
} // namespace middleend
