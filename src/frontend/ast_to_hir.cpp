#include <cstdint>
#include <memory>

#include "frontend/ast/ast.h"
#include "frontend/ast/expr.h"
#include "frontend/ast_to_hir.h"
#include "frontend/hir/hir.h"
#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {
    ASTToHIRVisitor::ASTToHIRVisitor(SymbolTable *old_table,
                                     SymbolTable *new_table)
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
        internal_counts.clear();
        result.push_back(makeLabel("entry"));
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::createScopedIdentifier(std::string symbol,
                                            uint64_t scope) {
        std::string scoped_symbol = symbol + "_" + std::to_string(scope);

        uint64_t id = new_table->addSymbol(scoped_symbol);
        scope_mappings[scope].insert(symbol);
        return std::make_unique<AtomIdentifier>(id);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::createUnscopedIdentifier(std::string symbol) {
        uint64_t id = new_table->addSymbol(symbol);
        scope_mappings.back().insert(symbol);
        return std::make_unique<AtomIdentifier>(id);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::resolveDeclarationScope(AtomIdentifier *a) {
        return createScopedIdentifier(a->toString(*old_table), cur_scope);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::resolveUseScope(AtomIdentifier *a) {
        uint64_t scope = -1;
        for (int i = scope_mappings.size() - 1; i >= 0; i--) {
            if (scope_mappings[i].count(a->toString(*old_table))) {
                scope = i;
                break;
            }
        }
        return createScopedIdentifier(a->toString(*old_table), scope);
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
        uint64_t count = internal_counts[name]++;
        std::string postfix = (count == 0) ? "" : std::to_string(count);
        std::string label_str = "__" + name + postfix;

        auto identifier = createUnscopedIdentifier(label_str);
        auto res = std::make_unique<hir::Label>(std::move(identifier));
        return res;
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::makeTemp(std::string name) {
        uint64_t count = internal_counts[name]++;
        std::string postfix = (count == 0) ? "" : std::to_string(count);
        std::string label_str = "__" + name + postfix;

        return createUnscopedIdentifier(label_str);
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::makeLabelAtom(hir::Label *l) {
        return std::make_unique<AtomIdentifier>(l->getName()->getValue());
    }

    void ASTToHIRVisitor::addReturnIfMissing(ast::Function &f) {
        // if f is main with return type int, implicit return 0
        // if f is void type, implicit return
        if (!result.empty() &&
            dynamic_cast<hir::InstructionReturn *>(result.back().get()))
            return;

        if (f.getName()->toString(*old_table) == "main") {
            auto zero = std::make_unique<AtomLiteral>(0);
            auto ret =
                std::make_unique<hir::InstructionReturn>(std::move(zero));
            result.push_back(std::move(ret));
            return;
        }

        // TODO: void function
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

    void ASTToHIRVisitor::visit(ast::InstructionExpr *i) {
        i->getExpr()->accept(this);
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclaration *i) {
        Type type = i->getType();
        auto variable = resolveDeclarationScope(i->getVariable().get());

        auto new_i = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(variable));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclarationAssign *i) {
        Type type = i->getType();
        auto variable = resolveDeclarationScope(i->getVariable().get());

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(variable));
        result.push_back(std::move(new_declare));

        auto variable2 = resolveUseScope(i->getVariable().get());
        i->getValue()->accept(this);
        auto value = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto new_assign = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable2), std::move(value));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssign *i) {
        auto variable = resolveUseScope(i->getVariable().get());
        i->getValue()->accept(this);
        auto value = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto new_i = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable), std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionOpAssign *i) {
        auto left_variable = resolveUseScope(i->getVariable().get());
        auto right_variable = resolveUseScope(i->getVariable().get());
        auto op = i->getOp();
        i->getValue()->accept(this);
        auto value = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto new_i = std::make_unique<hir::InstructionAssignBinaryOp>(
            std::move(left_variable), op, std::move(right_variable),
            std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn *i) {
        i->getValue()->accept(this);
        auto value = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto new_i = std::make_unique<hir::InstructionReturn>(std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionIf *i) {
        bool has_else = i->getFBranch() != nullptr;

        auto t_label = makeLabel("if_t");
        auto f_label = has_else ? makeLabel("if_f") : nullptr;
        auto cont_label = makeLabel("if_cont");

        auto t_atom = makeLabelAtom(t_label.get());
        auto f_atom =
            makeLabelAtom(has_else ? f_label.get() : cont_label.get());

        i->getCond()->accept(this);
        auto cond = std::make_unique<AtomIdentifier>(last_expr->getValue());
        auto cond_i = std::make_unique<hir::InstructionBranchCond>(
            std::move(cond), std::move(t_atom), std::move(f_atom));
        result.push_back(std::move(cond_i));

        result.push_back(std::move(t_label));
        i->getTBranch()->accept(this);

        if (has_else) {
            auto cont_atom = makeLabelAtom(cont_label.get());

            auto continue_i =
                std::make_unique<hir::InstructionBranch>(std::move(cont_atom));
            result.push_back(std::move(continue_i));

            result.push_back(std::move(f_label));
            i->getFBranch()->accept(this);
        }

        result.push_back(std::move(cont_label));
    }

    void ASTToHIRVisitor::visit(ast::InstructionWhile *i) {
        auto cond_label = makeLabel("while_cond");
        auto body_label = makeLabel("while_body");
        auto cont_label = makeLabel("while_cont");

        auto cond_atom = makeLabelAtom(cond_label.get());
        auto body_atom = makeLabelAtom(body_label.get());
        auto cont_atom = makeLabelAtom(cont_label.get());

        result.push_back(std::move(cond_label));
        i->getCond()->accept(this);
        auto cond = std::make_unique<AtomIdentifier>(last_expr->getValue());
        auto cond_i = std::make_unique<hir::InstructionBranchCond>(
            std::move(cond), std::move(body_atom), std::move(cont_atom));
        result.push_back(std::move(cond_i));

        result.push_back(std::move(body_label));
        i->getBody()->accept(this);
        auto continue_i =
            std::make_unique<hir::InstructionBranch>(std::move(cond_atom));
        result.push_back(std::move(continue_i));

        result.push_back(std::move(cont_label));
    }

    void ASTToHIRVisitor::visit(ast::Expr *e) {}

    void ASTToHIRVisitor::visit(ast::TerminalExpr *e) {
        auto type = Type::INT;
        auto var_declare = makeTemp("term");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = var_declare.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionAssignValue>(
            std::move(var_assign),
            std::move(resolveUseScope(e->getAtom().get())));

        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::ParenExpr *e) {
        e->getBody()->accept(this);
    }

    void ASTToHIRVisitor::visit(ast::CallExpr *e) {
        std::vector<std::unique_ptr<Atom>> args;
        for (auto &arg : e->getArguments()) {
            arg->accept(this);
            args.push_back(
                std::make_unique<AtomIdentifier>(last_expr->getValue()));
        }

        auto type = Type::INT;
        auto var_declare = makeTemp("call");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = var_declare.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionCallAssign>(
            std::move(var_assign),
            createUnscopedIdentifier(e->getCallee()->toString(*old_table)),
            std::move(args));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::UnaryOpExpr *e) {
        e->getValue()->accept(this);
        auto value = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto type = Type::INT;
        auto var_declare = makeTemp("un");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = var_declare.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionAssignUnaryOp>(
            std::move(var_assign), e->getOp(), std::move(value));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::BinaryOpExpr *e) {
        e->getLeft()->accept(this);
        auto left = std::make_unique<AtomIdentifier>(last_expr->getValue());

        e->getRight()->accept(this);
        auto right = std::make_unique<AtomIdentifier>(last_expr->getValue());

        auto type = Type::INT;
        auto var_declare = makeTemp("bin");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = var_declare.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            type, std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionAssignBinaryOp>(
            std::move(var_assign), e->getOp(), std::move(left),
            std::move(right));
        result.push_back(std::move(new_assign));
    }

    hir::Program astToHir(ast::Program &ast) {
        auto &old_table = ast.getSymbolTable();
        auto new_table = std::make_unique<SymbolTable>();

        std::vector<hir::Function> functions;
        ASTToHIRVisitor visitor(old_table.get(), new_table.get());
        for (ast::Function &f : ast.getFunctions()) {
            Type type = f.getType();

            auto name = visitor.createUnscopedIdentifier(
                f.getName()->toString(*old_table));

            std::vector<hir::Parameter> params;
            for (auto &[param_type, param_name] : f.getParameters()) {
                params.push_back({param_type, visitor.resolveDeclarationScope(
                                                  param_name.get())});
            }

            f.getBody()->accept(&visitor);
            visitor.addReturnIfMissing(f);
            auto body = visitor.getResult();

            hir::Function hir_function(type, std::move(name), std::move(params),
                                       std::move(body));
            functions.push_back(std::move(hir_function));
            visitor.clearResult();
        }

        hir::Program hir_program(std::move(functions), std::move(new_table));
        return std::move(hir_program);
    }
} // namespace frontend
