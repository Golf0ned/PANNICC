#include <cstdint>
#include <vector>

#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"
#include "frontend/ast_to_hir.h"


namespace frontend {
    AtomIdentifier* descopeIdentifier(AtomIdentifier* a, uint64_t scope,
            SymbolTable old_table, SymbolTable& new_table) {
        std::string symbol = a->toString(old_table);
        std::string descoped_symbol = symbol + "_" + std::to_string(scope);
        uint64_t id = new_table.addSymbol(descoped_symbol);
        return new AtomIdentifier(id);
    }

    Atom* descopeIfIdentifier(Atom* a, uint64_t scope, SymbolTable old_table, SymbolTable& new_table) {
        return a->isIdentifier()
            ? (Atom*)descopeIdentifier((AtomIdentifier*)a, scope, old_table, new_table)
            : (Atom*)new AtomLiteral(a->getValue());
    }

    ASTToHIRVisitor::ASTToHIRVisitor(SymbolTable old_table, SymbolTable& new_table)
        : scope_level(0), old_table(old_table), new_table(new_table) {};

    hir::Program astToHir(ast::Program& ast) {
        SymbolTable old_table = ast.getSymbolTable();
        SymbolTable new_table;

        std::vector<hir::Function> functions;
        ASTToHIRVisitor visitor(old_table, new_table);
        for (ast::Function& f : ast.getFunctions()) {
            Type type = f.getType();
            AtomIdentifier* name = descopeIdentifier(f.getName(), 0, old_table, new_table);
            // TODO: params

            visitor.visit(f.getBody());
            auto body = visitor.getResult();

            hir::Function hir_function(type, name, body);
            functions.push_back(hir_function);
        }

        hir::Program hir_program(functions, new_table);
        return hir_program;
    }


    std::vector<hir::Instruction*> ASTToHIRVisitor::getResult() {
        return result;
    }

    void ASTToHIRVisitor::visit(ast::Instruction* i) {
        // TODO: visit methods
        // TODO: scope-aware identifiers
    }

    void ASTToHIRVisitor::visit(ast::Scope* s) {
        // TODO: visit methods
        // TODO: scope-aware identifiers
        scope_level++;
        for (auto i : s->getInstructions()) {
            i->accept(this);
        }
        scope_level--;
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclaration* i) {
        Type type = i->getType();
        AtomIdentifier* variable = descopeIdentifier(i->getVariable(), scope_level, old_table, new_table);

        hir::InstructionDeclaration* new_i = new hir::InstructionDeclaration(type, variable);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignValue* i) {
        AtomIdentifier* variable = descopeIdentifier(i->getVariable(), scope_level, old_table, new_table);
        Atom* value = descopeIfIdentifier(i->getValue(), scope_level, old_table, new_table);

        hir::InstructionAssignValue* new_i = new hir::InstructionAssignValue(variable, value);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignBinaryOp* i) {
        AtomIdentifier* variable = descopeIdentifier(i->getVariable(), scope_level, old_table, new_table);
        BinaryOp op = i->getOp();
        Atom* left = descopeIfIdentifier(i->getLeft(), scope_level, old_table, new_table);
        Atom* right = descopeIfIdentifier(i->getRight(), scope_level, old_table, new_table);

        hir::InstructionAssignBinaryOp* new_i = new hir::InstructionAssignBinaryOp(variable, op, left, right);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn* i) {
        Atom* value = descopeIfIdentifier(i->getValue(), scope_level, old_table, new_table);

        hir::InstructionReturn* new_i = new hir::InstructionReturn(value);
        result.push_back(new_i);
    }
}
