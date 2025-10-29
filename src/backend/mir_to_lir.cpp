#include "backend/mir_to_lir.h"
#include "middleend/utils/traversal.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir) {
        // Generate trees per instruction
        // - make sure to add a node for copyToReg + copyFromReg
        TreeGenVisitor visitor(mir);
        for (auto &f : mir.getFunctions()) {
            auto linearized = middleend::traverseLeastBranches(f.get());
            for (auto *bb : linearized) {
                for (auto &i : bb->getInstructions()) {
                    i->accept(&visitor);
                }
            }
            // TODO: reset visitor?
        }

        // Merge trees

        // Tile trees

        // Final cleanup
    }

    TreeGenVisitor::TreeGenVisitor(middleend::mir::Program &p) { nir.run(p); }

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

        cur_context.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionCall *i) {}

    void TreeGenVisitor::visit(middleend::mir::InstructionAlloca *i) {
        // TODO: get size properly, this sucks a lot
        auto size = std::make_unique<ImmediateNode>(8);

        auto alloca = std::make_unique<AllocaNode>();
        alloca->setSize(std::move(size));

        // TODO: what do we do with the alloca ptr?
        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(alloca));

        cur_context.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionLoad *i) {
        auto ptr = resolveValue(i->getPtr());

        auto load = std::make_unique<LoadNode>();
        load->setPtr(std::move(ptr));

        auto reg =
            std::make_unique<RegisterNode>(std::to_string(nir.getNumber(i)));
        reg->setSource(std::move(load));

        cur_context.push_back(std::move(reg));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionStore *i) {
        auto source = resolveValue(i->getValue());
        auto ptr = resolveValue(i->getPtr());

        auto store = std::make_unique<StoreNode>();
        store->setSource(std::move(source));
        store->setPtr(std::move(ptr));

        cur_context.push_back(std::move(store));
    }

    void TreeGenVisitor::visit(middleend::mir::InstructionPhi *i) {}

    void TreeGenVisitor::visit(middleend::mir::TerminatorReturn *t) {
        auto source = resolveValue(t->getValue());

        auto ret = std::make_unique<ReturnNode>();
        ret->setSource(std::move(source));

        cur_context.push_back(std::move(ret));
    }

    void TreeGenVisitor::visit(middleend::mir::TerminatorBranch *t) {}

    void TreeGenVisitor::visit(middleend::mir::TerminatorCondBranch *t) {}
} // namespace backend
