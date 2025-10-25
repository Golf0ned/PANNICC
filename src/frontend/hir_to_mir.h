#pragma once

#include "frontend/hir/hir.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/value.h"

namespace frontend {
    middleend::mir::Program hirToMir(hir::Program &hir);

    class HIRToMIRVisitor : public hir::InstructionVisitor {
    public:
        HIRToMIRVisitor(middleend::mir::Type function_type,
                        middleend::mir::LiteralMap &literal_map);

        std::list<std::unique_ptr<middleend::mir::BasicBlock>> getResult();
        std::unique_ptr<middleend::mir::Value>
        addParameter(Type type, AtomIdentifier *name);
        middleend::mir::Value *resolveAtom(Atom *a);
        middleend::mir::Value *getLiteral(uint64_t value,
                                          middleend::mir::Type type);
        bool startOfBasicBlock();
        void createBasicBlock(
            std::unique_ptr<middleend::mir::Terminator> terminator);
        void connectBasicBlocks();

        void visit(hir::Instruction *i) override;
        void visit(hir::Label *l) override;
        void visit(hir::InstructionDeclaration *i) override;
        void visit(hir::InstructionAssignValue *i) override;
        void visit(hir::InstructionAssignUnaryOp *i) override;
        void visit(hir::InstructionAssignBinaryOp *i) override;
        void visit(hir::InstructionReturn *i) override;
        void visit(hir::InstructionCall *i) override;
        void visit(hir::InstructionCallAssign *i) override;
        void visit(hir::InstructionBranch *i) override;
        void visit(hir::InstructionBranchCond *i) override;

    private:
        middleend::mir::Type function_type;
        middleend::mir::InstructionAlloca *ret_alloca;
        std::list<std::unique_ptr<middleend::mir::Instruction>>
            cur_instructions;
        std::list<std::unique_ptr<middleend::mir::BasicBlock>> basic_blocks;
        middleend::mir::LiteralMap &literal_map;
        bool new_basic_block;
        std::vector<uint64_t> labels;
        std::unordered_map<uint64_t, middleend::mir::InstructionAlloca *>
            value_mappings;
        std::vector<
            std::pair<middleend::mir::Instruction *, std::vector<uint64_t>>>
            instruction_to_bbs;
    };
} // namespace frontend
