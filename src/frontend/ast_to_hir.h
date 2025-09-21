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

        std::vector<std::unique_ptr<hir::Instruction>> getResult();
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
        void visit(ast::InstructionIf *i) override;

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
        void addReturnIfMissing(ast::Function &f);

    private:
        std::vector<std::unique_ptr<hir::Instruction>> result;
        uint64_t cur_scope;
        std::vector<std::unordered_set<std::string>> scope_mappings;
        SymbolTable old_table;
        SymbolTable &new_table;
        std::unordered_map<std::string, uint64_t> label_counts;
    };
} // namespace frontend
