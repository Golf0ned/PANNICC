#pragma once

#include "frontend/hir/hir.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/value.h"

namespace mir = middleend::mir;

namespace frontend {
    mir::Program hirToMir(hir::Program &hir);

    class HIRToMIRVisitor : public hir::InstructionVisitor {
    public:
        HIRToMIRVisitor();

        std::vector<mir::BasicBlock> getResult();
        mir::Value *resolveAtom(Atom *a);
        void makeLabelIfMissing(hir::Instruction *i);
        void connectBasicBlocks();

        void visit(hir::Instruction *i) override;
        void visit(hir::Label *l) override;
        void visit(hir::InstructionDeclaration *i) override;
        void visit(hir::InstructionAssignValue *i) override;
        void visit(hir::InstructionAssignBinaryOp *i) override;
        void visit(hir::InstructionReturn *i) override;
        void visit(hir::InstructionCall *i) override;
        void visit(hir::InstructionCallAssign *i) override;
        void visit(hir::InstructionBranch *i) override;
        void visit(hir::InstructionBranchCond *i) override;

    private:
        std::vector<std::unique_ptr<mir::Instruction>> cur_instructions;
        std::vector<std::unique_ptr<mir::Literal>> cur_literals;
        std::vector<mir::BasicBlock> basic_blocks;
        std::unordered_map<uint64_t, mir::InstructionAlloca *> value_mappings;
        bool new_basic_block;
        std::vector<uint64_t> labels;
        std::vector<std::pair<mir::Instruction *, std::vector<uint64_t>>>
            instruction_to_bbs;
    };
} // namespace frontend
