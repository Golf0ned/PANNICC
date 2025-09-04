#pragma once

#include "frontend/hir/hir.h"
#include "middleend/mir/mir.h"

namespace mir = middleend::mir;

namespace frontend {
    mir::Program hirToMir(hir::Program &hir);

    class HIRToMIRVisitor : public hir::InstructionVisitor {
    public:
        std::vector<mir::BasicBlock> getResult();

        void visit(hir::InstructionDeclaration *i) override;
        void visit(hir::InstructionAssignValue *i) override;
        void visit(hir::InstructionAssignBinaryOp *i) override;
        void visit(hir::InstructionReturn *i) override;
        void visit(hir::InstructionCall *i) override;
        void visit(hir::InstructionCallAssign *i) override;

    private:
        std::vector<mir::BasicBlock> basic_blocks;
    };
} // namespace frontend
