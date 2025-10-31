#include "backend/mir_to_lir.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        // Generate trees per instruction
        TreeGenVisitor visitor(mir);
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseTraces(f.get());

            visitor.startFunction(f.get());
            for (auto *bb : linearized) {
                visitor.startBasicBlock(bb);
                for (auto &i : bb->getInstructions())
                    i->accept(&visitor);
            }
            visitor.endFunction();
        }

        auto program_trees = visitor.getResult();

        // Merge trees
        for (auto &function_trees : program_trees) {
            // TODO: merge trees
        }

        // Tile trees
        std::list<std::unique_ptr<lir::Instruction>> instructions;
        for (auto &function_trees : program_trees) {
            for (auto &tree : function_trees) {
                // TODO: tile trees
            }
        }

        // Final cleanup
        lir::Program lir(std::move(instructions));
        return std::move(lir);
    }

    TreeGenVisitor::TreeGenVisitor(middleend::mir::Program &p)
        : next_block(nullptr) {
        nir.run(p);
    }

    std::list<std::list<std::unique_ptr<Node>>> TreeGenVisitor::getResult() {
        return std::move(program_trees);
    }

    void TreeGenVisitor::startFunction(middleend::mir::Function *f) {
        function_name = f->getName();

        std::list<std::unique_ptr<lir::Instruction>> instructions;

        auto label = std::make_unique<lir::Label>(function_name);
        instructions.push_back(std::move(label));
        // TODO: push callee-saved registers? (maybe wait for regalloc)

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }

    void TreeGenVisitor::endFunction() {
        program_trees.push_back(std::move(function_trees));
        function_trees.clear();
    }

    void TreeGenVisitor::startBasicBlock(middleend::mir::BasicBlock *bb) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        std::string label_name =
            '.' + function_name + '_' + std::to_string(nir.getNumber(bb));
        auto label = std::make_unique<lir::Label>(label_name);
        instructions.push_back(std::move(label));

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }

    std::unique_ptr<Node>
    TreeGenVisitor::resolveValue(middleend::mir::Value *v) {
        auto literal = dynamic_cast<middleend::mir::Literal *>(v);
        if (literal)
            return std::make_unique<ImmediateNode>(literal->getValue());
        // TODO: function params
        return std::make_unique<RegisterNode>(std::to_string(nir.getNumber(v)));
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

        function_trees.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: push ret label?
        // TODO: push caller-saved registers
        // TODO: move params into registers
        // TODO: call function label
        // TODO: ret label?
        // TODO: restore caller-saved registers

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        // TODO: get size properly, this sucks a lot
        auto size = std::make_unique<ImmediateNode>(8);

        auto alloca = std::make_unique<AllocaNode>();
        alloca->setSize(std::move(size));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(alloca));

        function_trees.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveValue(i->getPtr());

        auto load = std::make_unique<LoadNode>();
        load->setPtr(std::move(ptr));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(load));

        function_trees.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionStore *i) {
        auto source = resolveValue(i->getValue());
        auto ptr = resolveValue(i->getPtr());

        auto store = std::make_unique<StoreNode>();
        store->setSource(std::move(source));
        store->setPtr(std::move(ptr));

        function_trees.push_back(std::move(store));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionPhi *i) {
        auto to = resolveValue(i);
        std::list<std::unique_ptr<Node>> from;
        for (auto &[_, v] : i->getPredecessors())
            from.push_back(resolveValue(v));

        auto phi = std::make_unique<PhiNode>(std::move(to), std::move(from));

        function_trees.push_back(std::move(phi));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        // TODO: restore callee-saved registers
        // TODO: reset stack pointer
        // TODO: pop base pointer
        // TODO: generate ret

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorBranch *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        if (t->getSuccessor() != next_block) {
            // TODO: generate branch
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorCondBranch *t) {
        std::list<std::unique_ptr<lir::Instruction>> instructions;

        if (t->getTSuccessor() == next_block)
            throw std::domain_error("true branch should never be next block");

        // TODO: generate cond jump for TSuccessor
        if (t->getFSuccessor() != next_block) {
            // TODO: generate jump for FSuccessor
        }

        auto assembly = std::make_unique<AsmNode>(std::move(instructions));

        function_trees.push_back(std::move(assembly));
    }
} // namespace backend
