#include "backend/codegen.h"
#include "backend/passes/legalize.h"

namespace backend {
    void CodeGenVisitor::generateFilePreamble() { result += "    .text"; }

    void CodeGenVisitor::generateFilePostamble() {
        result += "    .section    .note.GNU-stack,\"\",@progbits";
        result += "    .ident  PANNICC (https://github.com/Golf0ned/PANNICC)";
    }

    void CodeGenVisitor::generateFunctionPreamble(lir::Function *f) {
        result += '\n';
        result += "    .globl  " + f->getName() + "\n";
        result += "    .type   " + f->getName() + ", @function\n";
        result += f->getName() + ":";
    }

    void CodeGenVisitor::generateFunctionPostamble(lir::Function *f) {}

    void CodeGenVisitor::visit(lir::InstructionCall *i) {
        result += "\n        ";
        result += "call    " + i->getLabel();
    }

    std::string generateCode(lir::Program &lir) {
        legalize(lir);

        // TODO: validation?

        // TODO: generate entry point

        CodeGenVisitor cgv;

        cgv.generateFilePreamble();
        for (auto &f : lir.getFunctions()) {
            cgv.generateFunctionPreamble(f.get());

            for (auto &i : f->getInstructions())
                i->accept(&cgv);

            cgv.generateFunctionPostamble(f.get());
        }
        cgv.generateFilePostamble();

        return cgv.getResult();
    }
} // namespace backend
