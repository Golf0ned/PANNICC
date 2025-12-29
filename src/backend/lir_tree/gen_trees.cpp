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
        // Mark param registers OR pop registers off stack
        //
        auto &params = f->getParameters();
        auto &arg_registers = lir::getArgRegisters();
        for (size_t param_num = 0; param_num < params.size(); param_num++) {
            auto extend = lir::Extend::NONE;
            auto size = lir::DataSize::QUADWORD;
            auto src = param_num < 6
                           ? static_cast<lir::Operand *>(
                                 om->getRegister(arg_registers[param_num]))
                           : static_cast<lir::Operand *>(
                                 om->getStackArg(param_num - 6));
            auto dst = resolveOperand(params[param_num].get());
            auto mov = std::make_unique<lir::InstructionMov>(extend, size, size,
                                                             src, dst);
            instructions.push_back(std::move(mov));
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

        std::string label_name = resolveLabel(bb);
        auto label = std::make_unique<lir::Label>(label_name);
        instructions.push_back(std::move(label));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    std::string TreeGenVisitor::resolveLabel(middleend::mir::BasicBlock *bb) {
        auto bb_num = nir.getNumber(bb);
        return "." + function_name + '_' +
               (bb_num == -1 ? "entry" : std::to_string(bb_num));
    }

    std::unique_ptr<Node>
    TreeGenVisitor::resolveLeaf(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return std::make_unique<ImmediateNode>(literal->getValue());
        auto reg = om->getRegister(std::to_string(nir.getNumber(v)));
        return std::make_unique<RegisterNode>(reg, nullptr);
    }

    std::unique_ptr<Node>
    TreeGenVisitor::resolveStackVar(middleend::mir::Value *v) {
        auto name = std::to_string(nir.getNumber(v));
        auto offset = stack_variables.at(name);

        auto reg = om->getRegister(lir::RegisterNum::RSP);
        auto rsp = std::make_unique<RegisterNode>(reg, nullptr);
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

        auto om_reg = om->getRegister(std::to_string(nir.getNumber(i)));
        auto reg = std::make_unique<RegisterNode>(om_reg, std::move(op));

        function_trees.insertTree(std::move(reg), {left_leaf, right_leaf},
                                  false);
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {
        auto &arg_registers = lir::getArgRegisters();
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        //
        // Mark arg registers
        //
        std::vector<lir::Operand *> args;
        for (auto arg : i->getArguments())
            args.push_back(resolveOperand(arg));

        //
        // Call the function
        //
        auto function_name = i->getCallee()->getName();
        auto call =
            std::make_unique<lir::InstructionVirtualCall>(function_name, args);
        instructions.push_back(std::move(call));

        //
        // Move return value
        //
        auto size = lir::DataSize::DOUBLEWORD;
        auto src = om->getRegister(lir::RegisterNum::EAX);
        auto dst = resolveOperand(i);
        auto mov_ret = std::make_unique<lir::InstructionMov>(
            lir::Extend::NONE, size, size, src, dst);
        instructions.push_back(std::move(mov_ret));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: unhardcode size lol
        auto size = om->getImmediate(4);
        stack_variables.insert({std::to_string(nir.getNumber(i)), stack_space});
        stack_space += 4;

        // TODO: turn this into some temporary alloca instruction
        auto rsp = om->getRegister(lir::RegisterNum::RSP);
        auto allocate = std::make_unique<lir::InstructionBinaryOp>(
            lir::BinaryOp::SUB, lir::DataSize::QUADWORD, size, rsp);

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveStackVar(i->getPtr());

        auto ptr_leaf = ptr.get();

        auto load = std::make_unique<LoadNode>(std::move(ptr));

        auto om_reg = om->getRegister(std::to_string(nir.getNumber(i)));
        auto reg = std::make_unique<RegisterNode>(om_reg, std::move(load));

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

        // TODO: make copies in predecessor blocks?

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        //
        // Mark return register
        // TODO
        //

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
            auto jmp =
                std::make_unique<lir::InstructionJmp>(resolveLabel(succ));
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
                lir::invert(cond_code), resolveLabel(f_succ));
            instructions.push_back(std::move(cjmp));
        } else {
            auto cjmp = std::make_unique<lir::InstructionCJmp>(
                cond_code, resolveLabel(t_succ));
            instructions.push_back(std::move(cjmp));
            if (f_succ != next_block) {
                auto jmp =
                    std::make_unique<lir::InstructionJmp>(resolveLabel(f_succ));
                instructions.push_back(std::move(jmp));
            }
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));
        function_trees.insertAsm(std::move(assembly));
    }
} // namespace backend::lir_tree
