#include "middleend/transform/gvn.h"

namespace middleend {
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

void GVN::registerAnalyses(
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
