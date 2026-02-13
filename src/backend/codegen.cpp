#include "backend/codegen.h"

namespace backend {
    void CodeGenVisitor::generateFunctionLabel(lir::Function *f) {
        std::string label = f->getName() + ":";
        result += result.empty() ? label : "\n" + label;
    }

    void CodeGenVisitor::generatePreamble(lir::Function *f) {}

    void CodeGenVisitor::generatePostamble(lir::Function *f) {}

    void CodeGenVisitor::visit(lir::InstructionCall *i) {
        result += "\n        ";
        result += "call    " + i->getLabel();
    }

    std::string generateCode(lir::Program &lir) {
        // TODO: validation?

        CodeGenVisitor cgv;
        for (auto &f : lir.getFunctions()) {
            cgv.generateFunctionLabel(f.get());
            cgv.generatePreamble(f.get());

            for (auto &i : f->getInstructions())
                i->accept(&cgv);

            cgv.generatePostamble(f.get());
        }

        return cgv.getResult();
    }
} // namespace backend
