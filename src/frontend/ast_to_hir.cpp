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

    void ASTToHIRVisitor::visit(ast::Atom* a) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::AtomIdentifier* a) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::AtomLiteral* a) {
        // TODO: visit methods
    }


    void ASTToHIRVisitor::visit(ast::Instruction* i) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::Scope* s) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclaration* i) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignValue* i) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignBinaryOp* i) {
        // TODO: visit methods
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn* i) {
        // TODO: visit methods
    }
}
