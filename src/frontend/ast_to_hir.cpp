#include <cstdint>
#include <memory>

#include "frontend/ast/ast.h"
#include "frontend/ast_to_hir.h"
#include "frontend/hir/hir.h"
#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {
    ASTToHIRVisitor::ASTToHIRVisitor(SymbolTable old_table,
                                     SymbolTable &new_table)
        : cur_scope(0), old_table(old_table), new_table(new_table) {
        scope_mappings.emplace_back();
        result.push_back(makeLabel("entry"));
    };

    std::vector<std::unique_ptr<hir::Instruction>>
    ASTToHIRVisitor::getResult() {
        return std::move(result);
    }

    void ASTToHIRVisitor::clearResult() {
        result.clear();
        label_counts.clear();
        result.push_back(makeLabel("entry"));
    }

    void ASTToHIRVisitor::visit(ast::Instruction *i) {}

    void ASTToHIRVisitor::visit(ast::Scope *s) {
        cur_scope++;
        scope_mappings.emplace_back();
        for (auto &i : s->getInstructions()) {
            i->accept(this);
        }
        scope_mappings.pop_back();
        cur_scope--;
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclaration *i) {
        Type type = i->getType();
        auto variable = resolveDeclarationScope(i->getVariable().get());

        auto new_i = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(variable));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclarationAssignValue *i) {
        Type type = i->getType();
        auto variable = resolveDeclarationScope(i->getVariable().get());

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(variable));
        result.push_back(std::move(new_declare));

        auto variable2 = resolveUseScope(i->getVariable().get());
        auto value = resolveUseScope(i->getValue().get());

        auto new_assign = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable2), std::move(value));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignValue *i) {
        auto variable = resolveUseScope(i->getVariable().get());
        auto value = resolveUseScope(i->getValue().get());

        auto new_i = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable), std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssignBinaryOp *i) {
        auto variable = resolveUseScope(i->getVariable().get());
        BinaryOp op = i->getOp();
        auto left = resolveUseScope(i->getLeft().get());
        auto right = resolveUseScope(i->getRight().get());

        auto new_i = std::make_unique<hir::InstructionAssignBinaryOp>(
            std::move(variable), op, std::move(left), std::move(right));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn *i) {
        auto value = resolveUseScope(i->getValue().get());

        auto new_i = std::make_unique<hir::InstructionReturn>(std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionCall *i) {
        auto target =
            createUnscopedIdentifier(i->getTarget()->toString(old_table));

        auto new_i = std::make_unique<hir::InstructionCall>(std::move(target));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionCallAssign *i) {
        auto variable = resolveUseScope(i->getVariable().get());
        auto target =
            createUnscopedIdentifier(i->getTarget()->toString(old_table));

        auto new_i = std::make_unique<hir::InstructionCallAssign>(
            std::move(variable), std::move(target));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionIf *i) {
        bool has_else = i->getFBranch() != nullptr;

        auto t_label = makeLabel("if_t");
        auto f_label = has_else ? makeLabel("if_f") : nullptr;
        auto cont_label = makeLabel("if_cont");

        auto cond = resolveUseScope(i->getCond().get());
        auto cond_t_label =
            std::make_unique<AtomIdentifier>(t_label->getName()->getValue());
        auto cond_f_label = std::make_unique<AtomIdentifier>(
            has_else ? f_label->getName()->getValue()
                     : cont_label->getName()->getValue());
        auto cond_i = std::make_unique<hir::InstructionBranchCond>(
            std::move(cond), std::move(cond_t_label), std::move(cond_f_label));
        result.push_back(std::move(cond_i));

        result.push_back(std::move(t_label));
        i->getTBranch()->accept(this);

        if (has_else) {
            auto continue_label = std::make_unique<AtomIdentifier>(
                cont_label->getName()->getValue());
            auto continue_i = std::make_unique<hir::InstructionBranch>(
                std::move(continue_label));
            result.push_back(std::move(continue_i));

            result.push_back(std::move(f_label));
            i->getFBranch()->accept(this);
        }

        result.push_back(std::move(cont_label));
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::createScopedIdentifier(std::string symbol,
                                            uint64_t scope) {
        std::string scoped_symbol = symbol + "_" + std::to_string(scope);

        uint64_t id = new_table.addSymbol(scoped_symbol);
        scope_mappings[scope].insert(symbol);
        return std::make_unique<AtomIdentifier>(id);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::createUnscopedIdentifier(std::string symbol) {
        uint64_t id = new_table.addSymbol(symbol);
        scope_mappings.back().insert(symbol);
        return std::make_unique<AtomIdentifier>(id);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::resolveDeclarationScope(AtomIdentifier *a) {
        return createScopedIdentifier(a->toString(old_table), cur_scope);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::resolveUseScope(AtomIdentifier *a) {
        uint64_t scope = -1;
        for (int i = scope_mappings.size() - 1; i >= 0; i--) {
            if (scope_mappings[i].count(a->toString(old_table))) {
                scope = i;
                break;
            }
        }
        return createScopedIdentifier(a->toString(old_table), scope);
    }

    std::unique_ptr<Atom> ASTToHIRVisitor::resolveDeclarationScope(Atom *a) {
        return a->isIdentifier()
                   ? (std::unique_ptr<Atom>)resolveDeclarationScope(
                         (AtomIdentifier *)a)
                   : std::make_unique<AtomLiteral>(a->getValue());
    }

    std::unique_ptr<Atom> ASTToHIRVisitor::resolveUseScope(Atom *a) {
        return a->isIdentifier()
                   ? (std::unique_ptr<Atom>)resolveUseScope((AtomIdentifier *)a)
                   : std::make_unique<AtomLiteral>(a->getValue());
    }

    std::unique_ptr<hir::Label> ASTToHIRVisitor::makeLabel(std::string name) {
        uint64_t count = label_counts[name]++;
        std::string postfix = (count == 0) ? "" : std::to_string(count);
        std::string label_str = "__" + name + postfix;

        auto identifier = createUnscopedIdentifier(label_str);
        auto res = std::make_unique<hir::Label>(std::move(identifier));
        return res;
    }

    void ASTToHIRVisitor::addReturnIfMissing(ast::Function &f) {
        // if f is main with return type i32, implicit return 0
        // if f is void type, implicit return
        //
        // we have neither rn, so check main with ui64
        if (!result.empty() &&
            dynamic_cast<hir::InstructionReturn *>(result.back().get()))
            return;

        if (f.getName()->toString(old_table) == "main") {
            auto zero = std::make_unique<AtomLiteral>(0);
            auto ret =
                std::make_unique<hir::InstructionReturn>(std::move(zero));
            result.push_back(std::move(ret));
            return;
        }
    }

    hir::Program astToHir(ast::Program &ast) {
        SymbolTable old_table = ast.getSymbolTable();
        SymbolTable new_table;

        std::vector<hir::Function> functions;
        ASTToHIRVisitor visitor(old_table, new_table);
        for (ast::Function &f : ast.getFunctions()) {
            Type type = f.getType();

            auto name = visitor.createUnscopedIdentifier(
                f.getName()->toString(old_table));

            // TODO: params

            f.getBody()->accept(&visitor);
            visitor.addReturnIfMissing(f);
            auto body = visitor.getResult();

            hir::Function hir_function(type, std::move(name), std::move(body));
            functions.push_back(std::move(hir_function));
            visitor.clearResult();
        }

        hir::Program hir_program(std::move(functions), new_table);
        return hir_program;
    }
} // namespace frontend
