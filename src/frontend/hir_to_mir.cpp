#include "frontend/hir_to_mir.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"
#include "middleend/mir/type.h"

namespace mir = middleend::mir;

namespace frontend {
    std::unordered_map<uint64_t, mir::Function *> lowered_functions;

    mir::Program hirToMir(hir::Program &hir) {
        mir::LiteralMap literal_map;
        std::list<std::unique_ptr<mir::Function>> functions;

        for (auto &f : hir.getFunctions()) {
            mir::Type function_type = toMir(f.getType());
            HIRToMIRVisitor visitor(function_type, literal_map);

            std::vector<std::unique_ptr<mir::Value>> params;
            for (auto &[param_type, param_name] : f.getParameters()) {
                auto param = visitor.addParameter(param_type, param_name.get());
                params.push_back(std::move(param));
            }

            for (auto &i : f.getBody()) {
                if (visitor.startOfBasicBlock() &&
                    !dynamic_cast<hir::Label *>(i.get()))
                    continue;
                i->accept(&visitor);
            }
            visitor.connectBasicBlocks();
            auto basic_blocks = visitor.getResult();

            auto &name_atom = f.getName();
            std::string name = name_atom->toString(*hir.getSymbolTable().get());
            mir::BasicBlock *entry = basic_blocks.front().get();

            auto fun = std::make_unique<mir::Function>(
                function_type, name, std::move(params), std::move(basic_blocks),
                entry);
            lowered_functions[name_atom->getValue()] = fun.get();
            functions.push_back(std::move(fun));
        }
        return mir::Program(std::move(functions), std::move(literal_map));
    }

    HIRToMIRVisitor::HIRToMIRVisitor(mir::Type function_type,
                                     mir::LiteralMap &literal_map)
        : new_basic_block(true), function_type(function_type),
          literal_map(literal_map) {
        // TODO: handle void
        auto ret_val = std::make_unique<mir::InstructionAlloca>(function_type);
        ret_alloca = ret_val.get();
        cur_instructions.push_back(std::move(ret_val));
    }

    std::list<std::unique_ptr<mir::BasicBlock>> HIRToMIRVisitor::getResult() {
        return std::move(basic_blocks);
    }

    std::unique_ptr<mir::Value>
    HIRToMIRVisitor::addParameter(Type type, AtomIdentifier *name) {
        auto t = toMir(type);
        auto param = std::make_unique<mir::Value>(t);
        auto alloca = std::make_unique<mir::InstructionAlloca>(t);
        auto store =
            std::make_unique<mir::InstructionStore>(param.get(), alloca.get());
        value_mappings[name->getValue()] = alloca.get();

        cur_instructions.push_back(std::move(store));
        cur_instructions.push_back(std::move(alloca));

        return param;
    }

    mir::Value *HIRToMIRVisitor::resolveAtom(Atom *a) {
        auto type = toMir(Type::INT);
        if (a->isIdentifier()) {
            auto load = std::make_unique<mir::InstructionLoad>(
                type, value_mappings.at(a->getValue()));
            mir::InstructionLoad *load_ptr = load.get();
            cur_instructions.push_back(std::move(load));
            return load_ptr;
        } else {
            auto value = a->getValue();
            return getLiteral(value, type);
        }
    }

    mir::Value *HIRToMIRVisitor::getLiteral(uint64_t value, mir::Type type) {
        auto &typed_map = literal_map[type];
        if (typed_map.contains(value))
            return typed_map.at(value).get();

        auto literal = std::make_unique<mir::Literal>(type, value);
        auto literal_ptr = literal.get();

        typed_map[value] = std::move(literal);
        return literal_ptr;
    }

    bool HIRToMIRVisitor::startOfBasicBlock() { return new_basic_block; }

    void HIRToMIRVisitor::createBasicBlock(
        std::unique_ptr<mir::Terminator> terminator) {
        auto new_block = std::make_unique<mir::BasicBlock>(
            std::move(cur_instructions), std::move(terminator));
        basic_blocks.push_back(std::move(new_block));
        // cur_instructions.clear();
        new_basic_block = true;
    }

    void HIRToMIRVisitor::connectBasicBlocks() {
        // Close dangling block
        if (!new_basic_block) {
            auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
            instruction_to_bbs.push_back({branch.get(), {0}});
            createBasicBlock(std::move(branch));
        }

        // Generate return block
        // TODO: handle void
        auto ret_load =
            std::make_unique<mir::InstructionLoad>(function_type, ret_alloca);
        auto ret_terminator =
            std::make_unique<mir::TerminatorReturn>(ret_load.get());
        std::list<std::unique_ptr<mir::Instruction>> ret_body;
        ret_body.push_back(std::move(ret_load));
        auto ret_block = std::make_unique<mir::BasicBlock>(
            std::move(ret_body), std::move(ret_terminator));
        basic_blocks.push_back(std::move(ret_block));
        labels.push_back(0);

        // Add basic block pointers to terminators
        std::unordered_map<uint64_t, mir::BasicBlock *> label_to_bb;
        size_t i = 0;
        for (auto &bb : basic_blocks) {
            label_to_bb[labels[i++]] = bb.get();
        }

        for (auto [i, ids] : instruction_to_bbs) {
            auto branch = dynamic_cast<mir::TerminatorBranch *>(i);
            if (branch) {
                auto succ_id = ids[0];
                branch->setSuccessor(label_to_bb.at(succ_id));
            }

            auto cond_branch = dynamic_cast<mir::TerminatorCondBranch *>(i);
            if (cond_branch) {
                auto t_succ_id = ids[0], f_succ_id = ids[1];
                cond_branch->setTSuccessor(label_to_bb.at(t_succ_id));
                cond_branch->setFSuccessor(label_to_bb.at(f_succ_id));
            }
        }

        // Link basic blocks
        for (auto &bb : basic_blocks) {
            auto terminator = bb->getTerminator().get();
            auto branch = dynamic_cast<mir::TerminatorBranch *>(terminator);
            if (branch) {
                auto succ = branch->getSuccessor();
                bb->getSuccessors().addEdge(succ);
                succ->getPredecessors().addEdge(bb.get());
            }

            auto cond_branch =
                dynamic_cast<mir::TerminatorCondBranch *>(terminator);
            if (cond_branch) {
                auto t_succ = cond_branch->getTSuccessor();
                bb->getSuccessors().addEdge(t_succ);
                t_succ->getPredecessors().addEdge(bb.get()); // problematic line

                auto f_succ = cond_branch->getFSuccessor();
                bb->getSuccessors().addEdge(f_succ);
                f_succ->getPredecessors().addEdge(bb.get());
            }
        }
    }

    void HIRToMIRVisitor::visit(hir::Instruction *i) {}

    void HIRToMIRVisitor::visit(hir::Label *l) {
        if (!new_basic_block) {
            auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
            instruction_to_bbs.push_back(
                {branch.get(), {l->getName()->getValue()}});

            auto new_block = std::make_unique<mir::BasicBlock>(
                std::move(cur_instructions), std::move(branch));
            basic_blocks.push_back(std::move(new_block));
            // cur_instructions.clear();
        }

        labels.push_back(l->getName()->getValue());
        new_basic_block = false;
    }

    void HIRToMIRVisitor::visit(hir::InstructionDeclaration *i) {
        auto t = toMir(Type::INT);
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

    void HIRToMIRVisitor::visit(hir::InstructionAssignUnaryOp *i) {
        auto type = toMir(Type::INT);
        mir::Value *value = resolveAtom(i->getValue().get());

        mir::Value *un_op_res, *literal;
        std::unique_ptr<mir::InstructionBinaryOp> bin_op;
        switch (i->getOp()) {
        case UnaryOp::PLUS:
            // do nothing lol
            un_op_res = value;
            break;
        case UnaryOp::MINUS:
            // subtract value from 0
            literal = getLiteral(0, mir::Type::I32);
            bin_op = std::make_unique<mir::InstructionBinaryOp>(
                type, mir::BinaryOp::SUB, literal, value);
            un_op_res = bin_op.get();
            cur_instructions.push_back(std::move(bin_op));
            break;
        case UnaryOp::NOT:
            // xor with -1
            literal = getLiteral(-1, mir::Type::I32);
            bin_op = std::make_unique<mir::InstructionBinaryOp>(
                type, mir::BinaryOp::XOR, value, literal);
            un_op_res = bin_op.get();
            cur_instructions.push_back(std::move(bin_op));
            break;
        }

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(un_op_res, ptr);
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionAssignBinaryOp *i) {
        auto type = toMir(Type::INT);
        mir::Value *left = resolveAtom(i->getLeft().get());
        mir::Value *right = resolveAtom(i->getRight().get());
        mir::BinaryOp op = toMir(i->getOp());
        auto bin_op =
            std::make_unique<mir::InstructionBinaryOp>(type, op, left, right);

        mir::InstructionAlloca *ptr =
            value_mappings.at(i->getVariable()->getValue());
        auto store = std::make_unique<mir::InstructionStore>(bin_op.get(), ptr);

        cur_instructions.push_back(std::move(bin_op));
        cur_instructions.push_back(std::move(store));
    }

    void HIRToMIRVisitor::visit(hir::InstructionReturn *i) {
        auto retVal = resolveAtom(i->getValue().get());
        mir::InstructionAlloca *ptr = ret_alloca;
        auto store = std::make_unique<mir::InstructionStore>(retVal, ptr);
        cur_instructions.push_back(std::move(store));

        auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
        instruction_to_bbs.push_back({branch.get(), {0}});

        createBasicBlock(std::move(branch));
    }

    void HIRToMIRVisitor::visit(hir::InstructionCall *i) {
        auto type = toMir(Type::INT);
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call = std::make_unique<mir::InstructionCall>(type, callee);

        cur_instructions.push_back(std::move(call));
    }

    void HIRToMIRVisitor::visit(hir::InstructionCallAssign *i) {
        auto type = toMir(Type::INT);
        mir::Function *callee =
            lowered_functions.at(i->getCallee()->getValue());
        auto call = std::make_unique<mir::InstructionCall>(type, callee);

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

        createBasicBlock(std::move(branch));
    }

    void HIRToMIRVisitor::visit(hir::InstructionBranchCond *i) {
        auto cond = resolveAtom(i->getCmp().get());
        auto branch = std::make_unique<mir::TerminatorCondBranch>(
            std::move(cond), nullptr, nullptr);
        instruction_to_bbs.push_back(
            {branch.get(),
             {i->getTLabel()->getValue(), i->getFLabel()->getValue()}});

        createBasicBlock(std::move(branch));
    }
} // namespace frontend
