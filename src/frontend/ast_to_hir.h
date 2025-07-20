#pragma once

#include <vector>

#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"


namespace frontend {
    hir::Program astToHir(ast::Program& ast);

    class ASTToHIRVisitor : public ast::InstructionVisitor, public ast::AtomVisitor {
        public:
            std::vector<hir::Instruction*> getResult();

            void visit(ast::Atom* a) override;
            void visit(ast::AtomIdentifier* a) override;
            void visit(ast::AtomLiteral* a) override;

            void visit(ast::Instruction* i) override;
            void visit(ast::Scope* s) override;
            void visit(ast::InstructionDeclaration* i) override;
            void visit(ast::InstructionAssignValue* i) override;
            void visit(ast::InstructionAssignBinaryOp* i) override;
            void visit(ast::InstructionReturn* i) override;

        private:
            std::vector<hir::Instruction*> result;
    };
}
