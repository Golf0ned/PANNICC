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

        // TODO: push callee-saved registers? (maybe wait for regalloc)

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

    std::unique_ptr<Node>
    TreeGenVisitor::resolveValue(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return std::make_unique<ImmediateNode>(literal->getValue());
        return std::make_unique<RegisterNode>(std::to_string(nir.getNumber(v)));
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

        auto op = std::make_unique<OpNode>(i->getOp());
        op->setLeft(std::move(left));
        op->setRight(std::move(right));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(op));

        function_trees.emplace_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {
        // TODO: windows does it differently lmao
        // clang-format off
        lir::RegisterNum arg_registers[6] = {
            lir::RegisterNum::RDI,
            lir::RegisterNum::RSI,
            lir::RegisterNum::RDX,
            lir::RegisterNum::RCX,
            lir::RegisterNum::R8,
            lir::RegisterNum::R9,
        };
        // clang-format on

        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: push caller-saved registers

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
        // TODO: call function label
        // TODO: move return value into variable

        // TODO: restore caller-saved registers

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.emplace_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        // TODO: get size properly, this sucks a lot
        auto size = std::make_unique<ImmediateNode>(8);

        auto alloca = std::make_unique<AllocaNode>();
        alloca->setSize(std::move(size));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(alloca));

        function_trees.emplace_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveValue(i->getPtr());

        auto load = std::make_unique<LoadNode>();
        load->setPtr(std::move(ptr));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(load));

        function_trees.emplace_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionStore *i) {
        auto source = resolveValue(i->getValue());
        auto ptr = resolveValue(i->getPtr());

        auto store = std::make_unique<StoreNode>();
        store->setSource(std::move(source));
        store->setPtr(std::move(ptr));

        function_trees.emplace_back(std::move(store));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionPhi *i) {
        auto to = resolveValue(i);
        std::list<std::unique_ptr<Node>> from;
        for (auto &[_, v] : i->getPredecessors())
            from.push_back(resolveValue(v));

        auto phi = std::make_unique<PhiNode>(std::move(to), std::move(from));

        function_trees.emplace_back(std::move(phi));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: deallocate stack space?
        // TODO: restore callee-saved registers
        // TODO: generate ret

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
