#include "middleend/transform/gvn.h"

#include <format>

namespace middleend {
GVN::GVN() : next_num(0) {}

void GVN::run(mir::Program &p) {
    for (auto &f : p.getFunctions())
        run(f.get());
}

void GVN::run(mir::Function *f) {
    auto definition = dynamic_cast<mir::FunctionDefinition *>(f);
    if (!definition)
        return;
}

void GVN::dvnt(mir::BasicBlock *bb) {
    // TODO: make this out-of-order phi safe
    for (auto &i_iter : bb->getInstructions()) {
        auto i = i_iter.get();

        auto phi = dynamic_cast<mir::InstructionPhi *>(i);
        if (phi) {
            // TODO: if meaningless or redundant, map to equivalent

            // TODO: else, insert new value number
        }

        auto bin_op = dynamic_cast<mir::InstructionBinaryOp *>(i);
        if (bin_op) {
            // TODO: make equivalent expression
            // TODO: if found, map to equivalent
            // TODO: else, insert new value number
        }
    }

    for (auto &succ : bb->getSuccessors().getUniqueEdges()) {
        // TODO: adjust phi inputs
    }

    for (auto c : dt->getDominees(bb))
        dvnt(c);
}

std::string GVN::gvnHash(mir::Value *v) {
    // TODO: can we not use strings
    // TODO: do we need to account for sizes

    auto literal = dynamic_cast<mir::Literal *>(v);
    if (literal)
        return std::format("l{}", literal->getValue());

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

    return std::format("v{}", nir->getNumber(v));
}

void GVN::registerAnalyses(
    std::vector<std::unique_ptr<AnalysisPass>> &analyses) {
    for (auto &pass : analyses) {
        auto dt = dynamic_cast<DominatorTree *>(pass.get());
        if (dt) {
            this->dt = dt;
            required_analyses.push_back(dt);
            continue;
        }

        auto nir = dynamic_cast<NumberIR *>(pass.get());
        if (nir) {
            this->nir = nir;
            required_analyses.push_back(nir);
        }
    }

    if (!this->dt) {
        auto dt = std::make_unique<DominatorTree>();
        this->dt = dt.get();
        required_analyses.push_back(dt.get());
        analyses.push_back(std::move(dt));
    }

    if (!this->nir) {
        auto nir = std::make_unique<NumberIR>();
        this->nir = nir.get();
        required_analyses.push_back(nir.get());
        analyses.push_back(std::move(nir));
    }
}
} // namespace middleend
