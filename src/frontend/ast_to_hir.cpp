#include <cstdint>

#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"
#include "frontend/ast_to_hir.h"


namespace frontend {
    ASTToHIRVisitor::ASTToHIRVisitor(SymbolTable old_table, SymbolTable& new_table)
        : cur_scope(0), old_table(old_table), new_table(new_table) {
            scope_mappings.emplace_back();
        };

    std::vector<hir::Instruction*> ASTToHIRVisitor::getResult() {
        return result;
    }

    void ASTToHIRVisitor::clearResult() {
        result.clear();
    }

    void ASTToHIRVisitor::visit(ast::Instruction* i) {
    }

    void ASTToHIRVisitor::visit(ast::Scope* s) {
        cur_scope++;
        scope_mappings.emplace_back();
        for (auto i : s->getInstructions()) {
            i->accept(this);
        }
        scope_mappings.pop_back();
        cur_scope--;
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclaration* i) {
        Type type = i->getType();
        AtomIdentifier* variable = resolveDeclarationScope(i->getVariable());

        hir::InstructionDeclaration* new_i = new hir::InstructionDeclaration(type, variable);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclarationAssignValue* i) {
        Type type = i->getType();
        AtomIdentifier* variable1 = resolveDeclarationScope(i->getVariable());

        hir::InstructionDeclaration* new_declare = new hir::InstructionDeclaration(type, variable1);
        result.push_back(new_declare);

        AtomIdentifier* variable2 = resolveUseScope(i->getVariable());
        Atom* value = resolveUseScope(i->getValue());

        hir::InstructionAssignValue* new_assign = new hir::InstructionAssignValue(variable2, value);
        result.push_back(new_assign);
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignValue* i) {
        AtomIdentifier* variable = resolveUseScope(i->getVariable());
        Atom* value = resolveUseScope(i->getValue());

        hir::InstructionAssignValue* new_i = new hir::InstructionAssignValue(variable, value);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignBinaryOp* i) {
        AtomIdentifier* variable = resolveUseScope(i->getVariable());
        BinaryOp op = i->getOp();
        Atom* left = resolveUseScope(i->getLeft());
        Atom* right = resolveUseScope(i->getRight());

        hir::InstructionAssignBinaryOp* new_i = new hir::InstructionAssignBinaryOp(variable, op, left, right);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn* i) {
        Atom* value = resolveUseScope(i->getValue());

        hir::InstructionReturn* new_i = new hir::InstructionReturn(value);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionCall* i) {
        AtomIdentifier* target = resolveUseScope(i->getTarget());

        hir::InstructionCall* new_i = new hir::InstructionCall(target);
        result.push_back(new_i);
    }

    void ASTToHIRVisitor::visit(ast::InstructionCallAssign* i) {
        AtomIdentifier* variable = resolveUseScope(i->getVariable());
        AtomIdentifier* target = resolveUseScope(i->getTarget());

        hir::InstructionCallAssign* new_i = new hir::InstructionCallAssign(variable, target);
        result.push_back(new_i);
    }

    AtomIdentifier* ASTToHIRVisitor::createScopedIdentifier(std::string symbol, uint64_t scope, SymbolTable& new_table) {
        std::string scoped_symbol = symbol + "_" + std::to_string(scope);

        uint64_t id = new_table.addSymbol(scoped_symbol);
        scope_mappings.back().insert(symbol);
        return new AtomIdentifier(id);
    }
    
    AtomIdentifier* ASTToHIRVisitor::resolveDeclarationScope(AtomIdentifier* a) {
        return createScopedIdentifier(a->toString(old_table), cur_scope, new_table);
    }

    AtomIdentifier* ASTToHIRVisitor::resolveUseScope(AtomIdentifier* a) {
        uint64_t scope = -1;
        for (int i = scope_mappings.size() - 1; i >= 0; i--) {
            if (scope_mappings[i].count(a->toString(old_table))) {
                scope = i;
                break;
            }
        }
        return createScopedIdentifier(a->toString(old_table), scope, new_table);
    }

    Atom* ASTToHIRVisitor::resolveDeclarationScope(Atom* a) {
        return a->isIdentifier()
            ? (Atom*)resolveDeclarationScope((AtomIdentifier*)a)
            : new AtomLiteral(a->getValue());
    }

    Atom* ASTToHIRVisitor::resolveUseScope(Atom* a) {
        return a->isIdentifier()
            ? (Atom*)resolveUseScope((AtomIdentifier*)a)
            : new AtomLiteral(a->getValue());
    }


    hir::Program astToHir(ast::Program& ast) {
        SymbolTable old_table = ast.getSymbolTable();
        SymbolTable new_table;

        std::vector<hir::Function> functions;
        ASTToHIRVisitor visitor(old_table, new_table);
        for (ast::Function& f : ast.getFunctions()) {
            Type type = f.getType();
            AtomIdentifier* name = visitor.resolveDeclarationScope(f.getName());
            // TODO: params

            visitor.visit(f.getBody());
            auto body = visitor.getResult();

            hir::Function hir_function(type, name, body);
            functions.push_back(hir_function);
            visitor.clearResult();
        }

        hir::Program hir_program(functions, new_table);
        return hir_program;
    }
}
