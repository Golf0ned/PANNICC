#include <vector>

#include "frontend/ast_to_hir.h"
#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"


namespace frontend {
    hir::Program astToHir(ast::Program& ast) {
        hir::Program hir_program;

        ASTToHIRVisitor visitor;
        for (ast::Function& f : ast.getFunctions()) {
            // TODO: Convert function data (type, name, params)

            // TODO: Convert function body
            visitor.visit(f.getBody());

            // TODO: Create HIR function and add to program
        }

        return hir_program;
    }


    std::vector<hir::Instruction*> ASTToHIRVisitor::getResult() {
        return result;
    }

    // TODO: visit methods
}
