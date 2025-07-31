#pragma once

#include <cstdint>
#include <vector>

#include "frontend/utils/symbol_table.h"
#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"


namespace frontend {
    hir::Program astToHir(ast::Program& ast);

    class ASTToHIRVisitor : public ast::InstructionVisitor {
        public:
            ASTToHIRVisitor(SymbolTable old_table, SymbolTable& new_table);
            
            std::vector<hir::Instruction*> getResult();

            void visit(ast::Instruction* i) override;
            void visit(ast::Scope* s) override;
            void visit(ast::InstructionDeclaration* i) override;
            void visit(ast::InstructionAssignValue* i) override;
            void visit(ast::InstructionAssignBinaryOp* i) override;
            void visit(ast::InstructionReturn* i) override;

        private:
            std::vector<hir::Instruction*> result;
            uint64_t scope_level;
            SymbolTable old_table;
            SymbolTable& new_table;
    };
}
