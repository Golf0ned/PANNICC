#pragma once

#include "backend/lir/operand.h"
#include "backend/lir_tree/function_info.h"
#include "backend/lir_tree/node.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/mir/mir.h"

namespace backend::lir_tree {

    class TreeGenVisitor : public middleend::mir::InstructionVisitor {
    public:
        TreeGenVisitor(middleend::mir::Program &p, lir::OperandManager *om);

        std::list<FunctionTrees> getResult();
        std::unique_ptr<TreeInfo> getTreeInfo();
        std::vector<std::unique_ptr<FunctionInfo>> getFunctionInfo();

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

        FunctionTrees function_trees;
        std::unique_ptr<FunctionInfo> function_info;
        std::list<FunctionTrees> all_function_trees;
        std::vector<std::unique_ptr<FunctionInfo>> all_function_info;
        std::unique_ptr<TreeInfo> tree_info;
    };
} // namespace backend::lir_tree
