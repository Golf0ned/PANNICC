#pragma once

#include "backend/lir/operand.h"
#include "backend/lir_tree/node.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/mir/mir.h"

namespace backend::lir_tree {
    class FunctionInfo {
    public:
        FunctionInfo();

        // TODO: end this misery
        std::string name;
        uint64_t num_params;
        uint64_t stack_bytes;
    };

    class TreeGenVisitor : public middleend::mir::InstructionVisitor {
    public:
        TreeGenVisitor(middleend::mir::Program &p, lir::OperandManager *om);

        std::list<Forest> getResult();
        std::vector<FunctionInfo> getProgramInfo();

        void startFunction(middleend::mir::Function *f);
        void endFunction();
        void startBasicBlock(middleend::mir::BasicBlock *bb,
                             middleend::mir::BasicBlock *next_bb);
        std::string resolveLabel(middleend::mir::BasicBlock *bb);
        std::unique_ptr<Node> resolveLeaf(middleend::mir::Value *v);
        std::unique_ptr<Node> resolveStackVar(middleend::mir::Value *v);
        lir::Operand *resolveOperand(middleend::mir::Value *v);

        void visit(middleend::mir::InstructionBinaryOp *i) override;
        void visit(middleend::mir::InstructionCall *i) override;
        void visit(middleend::mir::InstructionAlloca *i) override;
        void visit(middleend::mir::InstructionLoad *i) override;
        void visit(middleend::mir::InstructionStore *i) override;
        void visit(middleend::mir::InstructionPhi *i) override;
        void visit(middleend::mir::InstructionParallelCopy *i) override;

        void visit(middleend::mir::TerminatorReturn *t) override;
        void visit(middleend::mir::TerminatorBranch *t) override;
        void visit(middleend::mir::TerminatorCondBranch *t) override;

    private:
        middleend::NumberIR nir;
        lir::OperandManager *om;

        middleend::mir::BasicBlock *next_block;
        std::unordered_map<std::string, uint64_t> stack_variables;
        FunctionInfo function_info;
        Forest function_trees;
        std::list<Forest> program_trees;
        std::vector<FunctionInfo> program_info;
    };
} // namespace backend::lir_tree
