#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {
    hir::Program astToHir(ast::Program &ast);

    class ASTToHIRVisitor : public ast::InstructionVisitor {
    public:
        ASTToHIRVisitor(SymbolTable old_table, SymbolTable &new_table);

        std::vector<hir::Instruction *> getResult();
        void clearResult();

        void visit(ast::Instruction *i) override;
        void visit(ast::Scope *s) override;
        void visit(ast::InstructionDeclaration *i) override;
        void visit(ast::InstructionDeclarationAssignValue *i) override;
        void visit(ast::InstructionAssignValue *i) override;
        void visit(ast::InstructionAssignBinaryOp *i) override;
        void visit(ast::InstructionReturn *i) override;
        void visit(ast::InstructionCall *i) override;
        void visit(ast::InstructionCallAssign *i) override;

        AtomIdentifier *createScopedIdentifier(std::string symbol,
                                               uint64_t scope,
                                               SymbolTable &new_table);
        AtomIdentifier *resolveDeclarationScope(AtomIdentifier *a);
        AtomIdentifier *resolveUseScope(AtomIdentifier *a);
        Atom *resolveDeclarationScope(Atom *a);
        Atom *resolveUseScope(Atom *a);

    private:
        std::vector<hir::Instruction *> result;
        uint64_t cur_scope;
        std::vector<std::unordered_set<std::string>> scope_mappings;
        SymbolTable old_table;
        SymbolTable &new_table;
    };
} // namespace frontend
