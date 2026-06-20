#include "middleend/transform/gvn.h"

#include <format>

namespace middleend {

void HashGVN::run(mir::Program &p) {
    for (auto &f : p.getFunctions())
        run(f.get());
}

void HashGVN::run(mir::Function *f) {
    auto definition = dynamic_cast<mir::FunctionDefinition *>(f);
    if (!definition)
        return;

    // TODO: worklist?
    dvnt(definition->getEntryBlock());

    // TODO: do pass replacing by hash
}

void HashGVN::dvnt(mir::BasicBlock *bb) {
    // TODO: make this out-of-order phi safe
    for (auto &i_iter : bb->getInstructions()) {
        auto i = i_iter.get();

        auto phi = dynamic_cast<mir::InstructionPhi *>(i);
        if (phi) {
            // TODO: check for back edges?

            auto hash = gvnHash(phi);
            if (!value_numberings.contains(hash))
                value_numberings[hash] = phi;

            continue;
        }

        auto bin_op = dynamic_cast<mir::InstructionBinaryOp *>(i);
        if (bin_op) {
            // TODO: substitute left and right
            auto left = bin_op->getLeft();
            auto right = bin_op->getRight();
            auto equivalent_op = std::make_unique<mir::InstructionBinaryOp>(
                bin_op->getType(), bin_op->getOp(), left, right);

            auto hash = gvnHash(equivalent_op.get());
            if (!value_numberings.contains(hash))
                value_numberings[hash] = bin_op;

            continue;
        }
    }

    for (auto &succ : bb->getSuccessors().getUniqueEdges()) {
        for (auto &i_iter : succ->getInstructions()) {
            auto i = i_iter.get();

            auto phi = dynamic_cast<mir::InstructionPhi *>(i);
            if (phi) {
                auto phi_val = phi->getPredecessors().at(bb);
                // TODO: replacement? do we want to do this during or after
            }

            break;
        }
    }

    for (auto c : dt->getDominees(bb))
        dvnt(c);
}

std::string HashGVN::gvnHash(mir::Value *v) {
    // TODO: can we not use strings
    // TODO: do we need to account for types

    auto literal = dynamic_cast<mir::Literal *>(v);
    if (literal)
        return std::format("l{}", static_cast<void *>(literal));

    auto bin_op = dynamic_cast<mir::InstructionBinaryOp *>(v);
    if (bin_op) {
        auto op = bin_op->getOp();
        auto left = bin_op->getLeft();
        auto right = bin_op->getRight();

        if (false
            // clang-format off
            || op == mir::BinaryOp::ADD
            || op == mir::BinaryOp::MUL
            || op == mir::BinaryOp::AND
            || op == mir::BinaryOp::OR
            || op == mir::BinaryOp::XOR
            // clang-format on
        ) {
            auto tmp = left;
            left = left < right ? left : right;
            right = tmp < right ? right : tmp;
        }

        return std::format("b{} {} {}", mir::toString(op), gvnHash(left),
                           gvnHash(right));
    }

    auto phi = dynamic_cast<mir::InstructionPhi *>(v);
    if (phi) {
        // TODO: what
    }

    return std::format("v{}", static_cast<void *>(v));
}

void HashGVN::registerAnalyses(
    std::vector<std::unique_ptr<AnalysisPass>> &analyses) {
    for (auto &pass : analyses) {
        auto dt = dynamic_cast<DominatorTree *>(pass.get());
        if (dt) {
            this->dt = dt;
            required_analyses.push_back(dt);
            continue;
        }
    }

    if (!this->dt) {
        auto dt = std::make_unique<DominatorTree>();
        this->dt = dt.get();
        required_analyses.push_back(dt.get());
        analyses.push_back(std::move(dt));
    }
}
} // namespace middleend
