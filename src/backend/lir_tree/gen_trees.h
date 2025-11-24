#pragma once

#include "backend/lir/operand.h"
#include "backend/lir_tree/node.h"
#include "middleend/analysis/number_ir.h"
#include "middleend/mir/mir.h"

namespace backend::lir_tree {
    class TreeGenVisitor : public middleend::mir::InstructionVisitor {
    public:
        TreeGenVisitor(middleend::mir::Program &p, lir::OperandManager *om);

        std::list<Forest> getResult();

        void startFunction(middleend::mir::Function *f);
        void endFunction();
        void startBasicBlock(middleend::mir::BasicBlock *bb,
                             middleend::mir::BasicBlock *next_bb);
        std::unique_ptr<Node> resolveLeaf(middleend::mir::Value *v);
        std::unique_ptr<Node> resolveStackVar(middleend::mir::Value *v);
        lir::Operand *resolveOperand(middleend::mir::Value *v);

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
        middleend::NumberIR nir;
        lir::OperandManager *om;
        std::string function_name;
        middleend::mir::BasicBlock *next_block;
        uint64_t stack_space;
        std::unordered_map<std::string, uint64_t> stack_variables;
        Forest function_trees;
        std::list<Forest> program_trees;
    };
} // namespace backend::lir_tree
