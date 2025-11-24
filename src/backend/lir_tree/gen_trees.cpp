#include "backend/lir_tree/gen_trees.h"
#include "backend/lir/data_size.h"

namespace backend::lir_tree {
    TreeGenVisitor::TreeGenVisitor(middleend::mir::Program &p,
                                   lir::OperandManager *om)
        : next_block(nullptr), om(om), stack_space(0) {
        nir.run(p);
    }

    std::list<Forest> TreeGenVisitor::getResult() {
        return std::move(program_trees);
    }

    void TreeGenVisitor::startFunction(middleend::mir::Function *f) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        //
        // Function label
        //
        function_name = f->getName();
        auto label = std::make_unique<lir::Label>(function_name);
        instructions.push_back(std::move(label));

        //
        // Push callee-saved registers
        //
        for (auto reg : lir::getCalleeSavedRegisters()) {
            auto src = om->getRegister(reg);
            auto push = std::make_unique<lir::InstructionPush>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(push));
            instructions.push_back(std::move(virt));
        }

        //
        // Move args to virtual registers
        // TODO: is this even a good idea?
        //
        auto &params = f->getParameters();
        auto &arg_registers = lir::getArgRegisters();
        for (int param_num = 0; param_num < params.size(); param_num++) {
            auto extend = lir::Extend::NONE;
            auto dst_size = lir::DataSize::QUADWORD;
            auto dst = resolveOperand(params[param_num].get());
            if (param_num < 6) {
                auto src = om->getRegister(arg_registers[param_num]);
                auto src_size = lir::DataSize::QUADWORD;
                auto mov = std::make_unique<lir::InstructionMov>(
                    extend, src_size, dst_size, src, dst);
                instructions.push_back(std::move(mov));
            } else {
                auto src_size = lir::fromMir(params[param_num]->getType());
                auto pop = std::make_unique<lir::InstructionPop>(src_size, dst);
                instructions.push_back(std::move(pop));
            }
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::endFunction() {
        program_trees.push_back(std::move(function_trees));
        function_trees = Forest();
        stack_space = 0;
    }

    void TreeGenVisitor::startBasicBlock(middleend::mir::BasicBlock *bb,
                                         middleend::mir::BasicBlock *next_bb) {
        next_block = next_bb;

        std::list<std::unique_ptr<lir::Instruction>> instructions;

        std::string label_name =
            '.' + function_name + '_' + std::to_string(nir.getNumber(bb));
        auto label = std::make_unique<lir::Label>(label_name);
        instructions.push_back(std::move(label));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    std::unique_ptr<Node>
    TreeGenVisitor::resolveLeaf(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return std::make_unique<ImmediateNode>(literal->getValue());
        return std::make_unique<RegisterNode>(std::to_string(nir.getNumber(v)),
                                              nullptr);
    }

    std::unique_ptr<Node>
    TreeGenVisitor::resolveStackVar(middleend::mir::Value *v) {
        auto name = std::to_string(nir.getNumber(v));
        auto offset = stack_variables.at(name);

        // TODO: please make this not a string
        auto rsp = std::make_unique<RegisterNode>("%rsp", nullptr);
        return std::make_unique<AddressNode>(std::move(rsp), nullptr, 0,
                                             offset);
    }

    lir::Operand *TreeGenVisitor::resolveOperand(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return om->getImmediate(literal->getValue());
        return om->getRegister(std::to_string(nir.getNumber(v)));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionBinaryOp *i) {
        auto left = resolveLeaf(i->getLeft());
        auto right = resolveLeaf(i->getRight());

        auto left_leaf = left.get();
        auto right_leaf = right.get();

        auto op = std::make_unique<OpNode>(i->getOp(), std::move(left),
                                           std::move(right));

        auto reg = std::make_unique<RegisterNode>(
            std::to_string(nir.getNumber(i)), std::move(op));

        function_trees.insertTree(std::move(reg), {left_leaf, right_leaf},
                                  false);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {
        auto &arg_registers = lir::getArgRegisters();
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        //
        // Push caller-saved registers
        //
        for (auto reg : lir::getCallerSavedRegisters()) {
            auto src = om->getRegister(reg);
            auto push = std::make_unique<lir::InstructionPush>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(push));
            instructions.push_back(std::move(virt));
        }

        //
        // Move args to parameter registers
        //
        auto args = i->getArguments();
        for (int arg_num = 0; arg_num < args.size(); arg_num++) {
            auto src = resolveOperand(args[arg_num]);
            auto src_size = lir::fromMir(args[arg_num]->getType());
            if (arg_num < 6) {
                auto dst = om->getRegister(arg_registers[arg_num]);
                auto dst_size = lir::DataSize::QUADWORD;
                auto extend = src_size == dst_size ? lir::Extend::NONE
                                                   : lir::Extend::SIGN;
                auto mov = std::make_unique<lir::InstructionMov>(
                    extend, src_size, dst_size, src, dst);
                instructions.push_back(std::move(mov));
            } else {
                auto push =
                    std::make_unique<lir::InstructionPush>(src_size, src);
                instructions.push_back(std::move(push));
            }
        }

        //
        // TODO: fix alignment
        //

        //
        // Call the function
        //
        auto function_name = i->getCallee()->getName();
        auto call = std::make_unique<lir::InstructionCall>(function_name);
        instructions.push_back(std::move(call));

        //
        // Move return value to virtual register
        //
        auto src = om->getRegister(lir::RegisterNum::RAX);
        auto src_size = lir::DataSize::QUADWORD;
        auto dst = resolveOperand(i);
        auto dst_size = lir::DataSize::QUADWORD;
        auto mov_ret = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, src_size, dst_size, src, dst);
        instructions.push_back(std::move(mov_ret));

        //
        // Restore caller-saved registers
        //
        for (auto reg : lir::getCallerSavedRegisters()) {
            auto src = om->getRegister(reg);
            auto pop = std::make_unique<lir::InstructionPop>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(pop));
            instructions.push_back(std::move(virt));
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: unhardcode size lol
        auto size = om->getImmediate(4);
        stack_variables.insert({std::to_string(nir.getNumber(i)), stack_space});
        stack_space += 4;

        auto rsp = om->getRegister(lir::RegisterNum::RSP);
        auto allocate = std::make_unique<lir::InstructionBinaryOp>(
            lir::BinaryOp::SUB, lir::DataSize::QUADWORD, size, rsp);

        auto virtual_allocate =
            std::make_unique<lir::InstructionVirtual>(std::move(allocate));
        instructions.push_back(std::move(virtual_allocate));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveStackVar(i->getPtr());

        auto ptr_leaf = ptr.get();

        auto load = std::make_unique<LoadNode>(std::move(ptr));

        auto reg = std::make_unique<RegisterNode>(
            std::to_string(nir.getNumber(i)), std::move(load));

        function_trees.insertTree(std::move(reg), {ptr_leaf}, true);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionStore *i) {
        auto source = resolveLeaf(i->getValue());
        auto ptr = resolveStackVar(i->getPtr());

        auto source_leaf = source.get();
        auto ptr_leaf = ptr.get();

        auto store =
            std::make_unique<StoreNode>(std::move(source), std::move(ptr));

        function_trees.insertTree(std::move(store), {source_leaf, ptr_leaf},
                                  true);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionPhi *i) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        auto to = resolveOperand(i);
        std::list<lir::Operand *> from;
        for (auto &[_, v] : i->getPredecessors())
            from.push_back(resolveOperand(v));

        auto phi = std::make_unique<lir::InstructionPhi>(std::move(from), to);
        instructions.push_back(std::move(phi));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        //
        // Move return value
        //
        auto src = resolveOperand(t->getValue());
        auto src_size = lir::DataSize::QUADWORD;
        auto dst = om->getRegister(lir::RegisterNum::RAX);
        auto dst_size = lir::DataSize::QUADWORD;
        auto mov_ret = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, src_size, dst_size, src, dst);
        instructions.push_back(std::move(mov_ret));

        //
        // Deallocate stack space
        //
        if (stack_space > 0) {
            auto rsp = om->getRegister(lir::RegisterNum::RSP);
            auto size = om->getImmediate(stack_space);
            auto deallocate = std::make_unique<lir::InstructionBinaryOp>(
                lir::BinaryOp::ADD, lir::DataSize::QUADWORD, size, rsp);
            auto virtual_deallocate = std::make_unique<lir::InstructionVirtual>(
                std::move(deallocate));
            instructions.push_back(std::move(virtual_deallocate));
        }

        //
        // Restore callee-saved registers
        //
        for (auto reg : lir::getCalleeSavedRegisters()) {
            auto src = om->getRegister(reg);
            auto pop = std::make_unique<lir::InstructionPop>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(pop));
            instructions.push_back(std::move(virt));
        }

        //
        // Return lmao
        //
        auto ret = std::make_unique<lir::InstructionRet>();
        instructions.push_back(std::move(ret));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorBranch *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        auto succ = t->getSuccessor();
        if (succ != next_block) {
            auto jmp = std::make_unique<lir::InstructionJmp>(
                std::to_string(nir.getNumber(succ)));
            instructions.push_back(std::move(jmp));
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorCondBranch *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: replace with test cond, cond
        auto cond = resolveOperand(t->getCond());
        auto cmp = std::make_unique<lir::InstructionCmp>(
            lir::DataSize::DOUBLEWORD, cond, om->getImmediate(0));
        instructions.push_back(std::move(cmp));

        auto t_succ = t->getTSuccessor(), f_succ = t->getFSuccessor();
        auto cond_code = lir::ConditionCode::EQ;

        //
        // If next_block is t_succ, inverse cjump to f_succ
        // If next_block is f_succ, cjump to t_succ
        // If next_block is neither, cjump to t_succ + jmp to f_succ
        //
        if (t_succ == next_block) {
            auto cjmp = std::make_unique<lir::InstructionCJmp>(
                lir::invert(cond_code), std::to_string(nir.getNumber(f_succ)));
            instructions.push_back(std::move(cjmp));
        } else {
            auto cjmp = std::make_unique<lir::InstructionCJmp>(
                cond_code, std::to_string(nir.getNumber(t_succ)));
            instructions.push_back(std::move(cjmp));
            if (f_succ != next_block) {
                auto jmp = std::make_unique<lir::InstructionJmp>(
                    std::to_string(nir.getNumber(f_succ)));
                instructions.push_back(std::move(jmp));
            }
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }
} // namespace backend::lir_tree
