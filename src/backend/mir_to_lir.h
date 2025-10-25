#pragma once

#include "backend/lir/lir.h"
#include "middleend/mir/mir.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir);

    class MIRToLIRVisitor : public middleend::mir::InstructionVisitor {
    public:
        MIRToLIRVisitor();

        std::list<std::unique_ptr<lir::Instruction>> getResult();

        virtual void visit(middleend::mir::InstructionBinaryOp *i);
        virtual void visit(middleend::mir::InstructionCall *i);
        virtual void visit(middleend::mir::InstructionAlloca *i);
        virtual void visit(middleend::mir::InstructionLoad *i);
        virtual void visit(middleend::mir::InstructionStore *i);
        virtual void visit(middleend::mir::InstructionPhi *i);

        virtual void visit(middleend::mir::TerminatorReturn *t);
        virtual void visit(middleend::mir::TerminatorBranch *t);
        virtual void visit(middleend::mir::TerminatorCondBranch *t);

    private:
    };
} // namespace backend
