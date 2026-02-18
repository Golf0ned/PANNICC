#include "backend/codegen.h"
#include "backend/passes/legalize.h"

namespace backend {
    void CodeGenVisitor::generateFunctionLabel(lir::Function *f) {
        std::string label = f->getName() + ":";
        result += result.empty() ? label : "\n" + label;
    }

    void CodeGenVisitor::visit(lir::InstructionCall *i) {
        result += "\n        ";
        result += "call    " + i->getLabel();
    }

    std::string generateCode(lir::Program &lir) {
        legalize(lir);

        // TODO: validation?

        // TODO: generate entry point

        CodeGenVisitor cgv;
        for (auto &f : lir.getFunctions()) {
            cgv.generateFunctionLabel(f.get());

            for (auto &i : f->getInstructions())
                i->accept(&cgv);
        }

        return cgv.getResult();
    }
} // namespace backend
