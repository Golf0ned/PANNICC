#pragma once

#include <cstdint>
#include <unordered_set>
#include <vector>

#include "frontend/ast/ast.h"
#include "frontend/ast/expr.h"
#include "frontend/hir/hir.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {
    hir::Program astToHir(ast::Program &ast);

    class ASTToHIRVisitor : public ast::InstructionVisitor,
                            public ast::ExprVisitor {
    public:
        ASTToHIRVisitor(SymbolTable *old_table, SymbolTable *new_table);

        std::vector<std::unique_ptr<hir::Instruction>> getResult();
        void clearResult();

        std::unique_ptr<AtomIdentifier>
        createScopedIdentifier(std::string symbol, uint64_t scope);
        std::unique_ptr<AtomIdentifier>
        createUnscopedIdentifier(std::string symbol);
        std::unique_ptr<AtomIdentifier>
        resolveDeclarationScope(AtomIdentifier *a);
        std::unique_ptr<Atom> resolveDeclarationScope(Atom *a);
        std::unique_ptr<AtomIdentifier> resolveUseScope(AtomIdentifier *a);
        std::unique_ptr<Atom> resolveUseScope(Atom *a);
        std::unique_ptr<hir::Label> makeLabel(std::string name);
        std::unique_ptr<AtomIdentifier> makeLabelAtom(hir::Label *l);
        std::unique_ptr<AtomIdentifier> makeTemp(std::string name);
        void addReturnIfMissing(ast::Function &f);

        void visit(ast::Instruction *i) override;
        void visit(ast::Scope *s) override;
        void visit(ast::InstructionExpr *i) override;
        void visit(ast::InstructionDeclaration *i) override;
        void visit(ast::InstructionDeclarationAssign *i) override;
        void visit(ast::InstructionAssign *i) override;
        void visit(ast::InstructionReturn *i) override;
        void visit(ast::InstructionIf *i) override;
        void visit(ast::InstructionWhile *i) override;

        void visit(ast::Expr *e) override;
        void visit(ast::TerminalExpr *e) override;
        void visit(ast::ParenExpr *e) override;
        void visit(ast::CallExpr *e) override;
        void visit(ast::UnaryOpExpr *e) override;
        void visit(ast::BinaryOpExpr *e) override;

    private:
        std::vector<std::unique_ptr<hir::Instruction>> result;
        AtomIdentifier *last_expr;
        uint64_t cur_scope;
        std::vector<std::unordered_set<std::string>> scope_mappings;
        SymbolTable *old_table;
        SymbolTable *new_table;
        std::unordered_map<std::string, uint64_t> internal_counts;
    };
} // namespace frontend
