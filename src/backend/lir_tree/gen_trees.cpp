#include "backend/lir_tree/gen_trees.h"

namespace backend::lir_tree {
    TreeGenVisitor::TreeGenVisitor(middleend::mir::Program &p,
                                   lir::OperandManager &om)
        : next_block(nullptr), om(om) {
        nir.run(p);
    }

    std::list<std::list<Tree>> TreeGenVisitor::getResult() {
        return std::move(program_trees);
    }

    void TreeGenVisitor::startFunction(middleend::mir::Function *f) {
        function_name = f->getName();

        std::list<std::unique_ptr<lir::Instruction>> instructions;

        auto label = std::make_unique<lir::Label>(function_name);
        instructions.push_back(std::move(label));

        for (auto reg : lir::getCalleeSavedRegisters()) {
            auto src = om.getRegister(reg);
            auto push = std::make_unique<lir::InstructionPush>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(push));
            instructions.push_back(std::move(virt));
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.emplace_back(std::move(assembly));
    }

    void TreeGenVisitor::endFunction() {
        program_trees.push_back(std::move(function_trees));
        function_trees.clear();
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
        function_trees.emplace_back(std::move(assembly));
    }

    std::shared_ptr<Node>
    TreeGenVisitor::resolveValue(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return std::make_shared<ImmediateNode>(literal->getValue());
        return std::make_shared<RegisterNode>(std::to_string(nir.getNumber(v)));
    }

    lir::Operand *TreeGenVisitor::resolveOperand(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return om.getImmediate(literal->getValue());
        return om.getRegister(std::to_string(nir.getNumber(v)));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionBinaryOp *i) {
        auto left = resolveValue(i->getLeft());
        auto right = resolveValue(i->getRight());

        auto left_leaf = left;
        auto right_leaf = right;

        auto op = std::make_shared<OpNode>(i->getOp());
        op->setLeft(std::move(left));
        op->setRight(std::move(right));

        auto reg =
            std::make_shared<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(op));

        auto &tree = function_trees.emplace_back(std::move(reg));
        tree.getLeaves().insert(left_leaf);
        tree.getLeaves().insert(right_leaf);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {
        auto &arg_registers = lir::getArgRegisters();
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        for (auto reg : lir::getCallerSavedRegisters()) {
            auto src = om.getRegister(reg);
            auto push = std::make_unique<lir::InstructionPush>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(push));
            instructions.push_back(std::move(virt));
        }

        auto args = i->getArguments();
        for (int arg_num = 0; arg_num < args.size(); arg_num++) {
            auto src = resolveOperand(args[arg_num]);
            auto src_size = lir::fromMir(i->getType());
            if (arg_num < 6) {
                auto dst = om.getRegister(arg_registers[arg_num]);
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

        // TODO: allocate stack space?

        auto function_name = i->getCallee()->getName();
        auto call = std::make_unique<lir::InstructionCall>(function_name);
        instructions.push_back(std::move(call));

        auto src = om.getRegister(lir::RegisterNum::RAX);
        auto src_size = lir::DataSize::QUADWORD;
        auto dst = resolveOperand(i);
        auto dst_size = lir::DataSize::QUADWORD;
        auto mov_ret = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, src_size, dst_size, src, dst);
        instructions.push_back(std::move(mov_ret));

        for (auto reg : lir::getCallerSavedRegisters()) {
            auto src = om.getRegister(reg);
            auto pop = std::make_unique<lir::InstructionPop>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(pop));
            instructions.push_back(std::move(virt));
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.emplace_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        // TODO: get size properly, this sucks a lot
        auto size = std::make_shared<ImmediateNode>(8);

        auto size_leaf = size;

        auto alloca = std::make_shared<AllocaNode>();
        alloca->setSize(std::move(size));

        auto reg =
            std::make_shared<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(alloca));

        function_trees.emplace_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveValue(i->getPtr());

        auto ptr_leaf = ptr;

        auto load = std::make_shared<LoadNode>();
        load->setPtr(std::move(ptr));

        auto reg =
            std::make_shared<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(load));

        auto &tree = function_trees.emplace_back(std::move(reg));
        tree.getLeaves().insert(ptr_leaf);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionStore *i) {
        auto source = resolveValue(i->getValue());
        auto ptr = resolveValue(i->getPtr());

        auto source_leaf = source;
        auto ptr_leaf = ptr;

        auto store = std::make_shared<StoreNode>();
        store->setSource(std::move(source));
        store->setPtr(std::move(ptr));

        auto &tree = function_trees.emplace_back(std::move(store));
        tree.getLeaves().insert(source_leaf);
        tree.getLeaves().insert(ptr_leaf);
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
        function_trees.emplace_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: deallocate stack space?

        for (auto reg : lir::getCalleeSavedRegisters()) {
            auto src = om.getRegister(reg);
            auto pop = std::make_unique<lir::InstructionPop>(
                lir::DataSize::QUADWORD, src);
            auto virt =
                std::make_unique<lir::InstructionVirtual>(std::move(pop));
            instructions.push_back(std::move(virt));
        }

        auto ret = std::make_unique<lir::InstructionRet>();
        instructions.push_back(std::move(ret));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.emplace_back(std::move(assembly));
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
        function_trees.emplace_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorCondBranch *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: replace with test cond, cond
        auto cond = resolveOperand(t->getCond());
        auto cmp = std::make_unique<lir::InstructionCmp>(
            lir::DataSize::DOUBLEWORD, cond, om.getImmediate(0));
        instructions.push_back(std::move(cmp));

        auto t_succ = t->getTSuccessor(), f_succ = t->getFSuccessor();
        auto cond_code = lir::ConditionCode::EQ;

        // If next_block is t_succ, inverse cjump to f_succ
        // If next_block is f_succ, cjump to t_succ
        // If next_block is neither, cjump to t_succ + jmp to f_succ
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
        function_trees.emplace_back(std::move(assembly));
    }
} // namespace backend::lir_tree
