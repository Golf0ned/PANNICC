#include "frontend/hir_to_mir.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/type.h"
#include <iostream>

namespace mir = middleend::mir;

namespace frontend {
    std::unordered_map<uint64_t, mir::Function *> lowered_functions;

    HIRToMIRVisitor::HIRToMIRVisitor(mir::Type function_type)
        : new_basic_block(true), function_type(function_type) {
        // TODO: handle void
        auto ret_val = std::make_unique<mir::InstructionAlloca>(function_type);
        ret_alloca = ret_val.get();
        cur_instructions.push_back(std::move(ret_val));
    }

    mir::Program hirToMir(hir::Program &hir) {
        std::vector<mir::Function> functions;
        functions.reserve(hir.getFunctions().size());

        for (auto &f : hir.getFunctions()) {
            mir::Type function_type = toMir(f.getType());

            HIRToMIRVisitor visitor(function_type);
            for (auto &i : f.getBody()) {
                if (visitor.startOfBasicBlock() &&
                    !dynamic_cast<hir::Label *>(i.get()))
                    continue;
                i->accept(&visitor);
            }
            visitor.connectBasicBlocks();
            std::vector<mir::BasicBlock> basic_blocks = visitor.getResult();

            auto &nameAtom = f.getName();
            std::string name = nameAtom->toString(hir.getSymbolTable());
            functions.emplace_back(function_type, name,
                                   std::move(basic_blocks));
            lowered_functions[nameAtom->getValue()] = &functions.back();
        }
        return mir::Program(std::move(functions));
    }

    std::vector<mir::BasicBlock> HIRToMIRVisitor::getResult() {
        return std::move(basic_blocks);
    }

    mir::Value *HIRToMIRVisitor::resolveAtom(Atom *a) {
        if (a->isIdentifier()) {
            auto load = std::make_unique<mir::InstructionLoad>(
                mir::Type::I64, value_mappings.at(a->getValue()));
            mir::InstructionLoad *load_ptr = load.get();
            cur_instructions.push_back(std::move(load));
            return load_ptr;
        } else {
            auto literal =
                std::make_unique<mir::Literal>(mir::Type::I64, a->getValue());
            mir::Literal *literal_ptr = literal.get();
            cur_literals.push_back(std::move(literal));
            return literal_ptr;
        }
    }

    bool HIRToMIRVisitor::startOfBasicBlock() { return new_basic_block; }

    void HIRToMIRVisitor::connectBasicBlocks() {
        // Close dangling block
        if (!new_basic_block) {
            auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
            instruction_to_bbs.push_back({branch.get(), {0}});

            basic_blocks.emplace_back(std::move(cur_instructions),
                                      std::move(branch),
                                      std::move(cur_literals));
            cur_instructions.clear();
            cur_literals.clear();
            new_basic_block = true;
        }

        // Generate return block
        // TODO: handle void
        auto ret_load =
            std::make_unique<mir::InstructionLoad>(function_type, ret_alloca);
        auto ret_terminator =
            std::make_unique<mir::TerminatorReturn>(ret_load.get());
        std::vector<std::unique_ptr<mir::Instruction>> ret_body;
        ret_body.push_back(std::move(ret_load));
        std::vector<std::unique_ptr<mir::Literal>> ret_literals;
        basic_blocks.emplace_back(std::move(ret_body),
                                  std::move(ret_terminator),
                                  std::move(ret_literals));
        labels.push_back(0);

        // Add basic block pointers to terminators
        std::unordered_map<uint64_t, mir::BasicBlock *> label_to_bb;
        for (int i = 0; i < labels.size(); i++) {
            label_to_bb[labels[i]] = &basic_blocks[i];
        }

        for (auto [i, ids] : instruction_to_bbs) {
            auto branch = dynamic_cast<mir::TerminatorBranch *>(i);
            if (branch) {
                auto successor_id = ids[0];
                branch->setSuccessor(label_to_bb[successor_id]);
            }

            auto cond_branch = dynamic_cast<mir::TerminatorCondBranch *>(i);
            if (cond_branch) {
                auto t_successor_id = ids[0], f_successor_id = ids[1];
                cond_branch->setTSuccessor(label_to_bb[t_successor_id]);
                cond_branch->setFSuccessor(label_to_bb[f_successor_id]);
            }
        }

        // Link basic blocks
        for (auto &bb : basic_blocks) {
            auto terminator = bb.getTerminator().get();
            auto branch = dynamic_cast<mir::TerminatorBranch *>(terminator);
            if (branch) {
                bb.getSuccessors().push_back(branch->getSuccessor());
                branch->getSuccessor()->getPredecessors().push_back(&bb);
            }

            auto cond_branch =
                dynamic_cast<mir::TerminatorCondBranch *>(terminator);
            if (cond_branch) {
                bb.getSuccessors().push_back(cond_branch->getTSuccessor());
                cond_branch->getTSuccessor()->getPredecessors().push_back(&bb);
                bb.getSuccessors().push_back(cond_branch->getFSuccessor());
                cond_branch->getFSuccessor()->getPredecessors().push_back(&bb);
            }
        }
    }

    void HIRToMIRVisitor::visit(hir::Instruction *i) {}

    void HIRToMIRVisitor::visit(hir::Label *l) {
        if (!new_basic_block) {
            auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
            instruction_to_bbs.push_back(
                {branch.get(), {l->getName()->getValue()}});

            basic_blocks.emplace_back(std::move(cur_instructions),
                                      std::move(branch),
                                      std::move(cur_literals));
            cur_instructions.clear();
            cur_literals.clear();
        }

        labels.push_back(l->getName()->getValue());
        new_basic_block = false;
    }

    void HIRToMIRVisitor::visit(hir::InstructionDeclaration *i) {
        mir::Type t = toMir(i->getType());
        auto alloca = std::make_unique<mir::InstructionAlloca>(t);
        value_mappings[i->getVariable()->getValue()] = alloca.get();

        cur_instructions.push_back(std::move(alloca));
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignValue *i) {
        auto value = resolveAtom(i->getValue().get());
        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(value, ptr);

        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignBinaryOp *i) {
        mir::Value *left = resolveAtom(i->getLeft().get());
        mir::Value *right = resolveAtom(i->getRight().get());
        mir::BinaryOp op = toMir(i->getOp());
        auto binOp = std::make_unique<mir::InstructionBinaryOp>(
            mir::Type::I64, op, left, right);

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(binOp.get(), ptr);

        cur_instructions.push_back(std::move(binOp));
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {
        auto retVal = resolveAtom(i->getValue().get());
        mir::InstructionAlloca *ptr = ret_alloca;
        auto store = std::make_unique<mir::InstructionStore>(retVal, ptr);
        cur_instructions.push_back(std::move(store));

        auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
        instruction_to_bbs.push_back({branch.get(), {0}});

        basic_blocks.emplace_back(std::move(cur_instructions),
                                  std::move(branch), std::move(cur_literals));
        cur_instructions.clear();
        cur_literals.clear();
        new_basic_block = true;
    }

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call =
            std::make_unique<mir::InstructionCall>(mir::Type::I64, callee);

        cur_instructions.push_back(std::move(call));
    }

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call =
            std::make_unique<mir::InstructionCall>(mir::Type::I64, callee);

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(call.get(), ptr);

        cur_instructions.push_back(std::move(call));
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionBranch *i) {
        auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
        instruction_to_bbs.push_back(
            {branch.get(), {i->getLabel()->getValue()}});

        basic_blocks.emplace_back(std::move(cur_instructions),
                                  std::move(branch), std::move(cur_literals));
        cur_instructions.clear();
        cur_literals.clear();
        new_basic_block = true;
    }

    void HIRToMIRVisitor::visit(hir::InstructionBranchCond *i) {
        auto cond = resolveAtom(i->getCmp().get());
        auto branch = std::make_unique<mir::TerminatorCondBranch>(
            std::move(cond), nullptr, nullptr);
        instruction_to_bbs.push_back(
            {branch.get(),
             {i->getTLabel()->getValue(), i->getFLabel()->getValue()}});

        basic_blocks.emplace_back(std::move(cur_instructions),
                                  std::move(branch), std::move(cur_literals));
        cur_instructions.clear();
        cur_literals.clear();
        new_basic_block = true;
    }
} // namespace frontend
