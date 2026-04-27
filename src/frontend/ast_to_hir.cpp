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
        var_type_mappings.clear();
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

    std::unique_ptr<Atom> ASTToHIRVisitor::copyAtom(Atom *atom) {
        return atom->isIdentifier()
                   ? static_cast<std::unique_ptr<Atom>>(
                         std::make_unique<AtomIdentifier>(atom->getValue()))
                   : std::make_unique<AtomLiteral>(atom->getValue());
    }

    std::unique_ptr<AtomIdentifier>
    ASTToHIRVisitor::makeLabelAtom(hir::Label *l) {
        return std::make_unique<AtomIdentifier>(l->getName()->getValue());
    }

    void ASTToHIRVisitor::addReturnIfMissing(ast::FunctionDefinition *f) {
        // if f is main with return type int, implicit return 0
        // if f is void type, implicit return
        if (!result.empty() &&
            dynamic_cast<hir::InstructionReturn *>(result.back().get()))
            return;

        if (f->getName()->toString(*old_table) == "main") {
            auto zero = std::make_unique<AtomLiteral>(0);
            auto ret =
                std::make_unique<hir::InstructionReturn>(std::move(zero));
            result.push_back(std::move(ret));
            return;
        }

        // TODO: void function
    }

    void ASTToHIRVisitor::mapFunctionType(AtomIdentifier *function,
                                          Type *type) {
        function_type_mappings[function->getValue()] = type;
    }

    void ASTToHIRVisitor::mapVariableType(AtomIdentifier *var, Type *type) {
        var_type_mappings[var->getValue()] = type;
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
        auto type = std::move(i->getType());
        auto variable = resolveDeclarationScope(i->getVariable().get());
        var_type_mappings[variable->getValue()] = type.get();

        auto new_i = std::make_unique<hir::InstructionDeclaration>(
            std::move(type), std::move(variable));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionDeclarationAssign *i) {
        auto type = std::move(i->getType());
        auto variable = resolveDeclarationScope(i->getVariable().get());
        var_type_mappings[variable->getValue()] = type.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            std::move(type), std::move(variable));
        result.push_back(std::move(new_declare));

        auto variable2 = resolveUseScope(i->getVariable().get());
        i->getValue()->accept(this);
        auto value = std::move(last_expr);

        auto new_assign = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable2), std::move(value));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::InstructionAssign *i) {
        i->getVariable()->accept(this);
        auto variable = std::unique_ptr<AtomIdentifier>(
            static_cast<AtomIdentifier *>(last_expr.release()));

        i->getValue()->accept(this);
        auto value = std::move(last_expr);

        auto new_i = std::make_unique<hir::InstructionAssignValue>(
            std::move(variable), std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionOpAssign *i) {
        i->getVariable()->accept(this);
        auto left_variable = std::unique_ptr<AtomIdentifier>(
            static_cast<AtomIdentifier *>(last_expr.release()));

        i->getVariable()->accept(this);
        auto right_variable = std::unique_ptr<AtomIdentifier>(
            static_cast<AtomIdentifier *>(last_expr.release()));

        auto op = i->getOp();
        i->getValue()->accept(this);
        auto value = std::move(last_expr);

        auto new_i = std::make_unique<hir::InstructionAssignBinaryOp>(
            std::move(left_variable), op, std::move(right_variable),
            std::move(value));
        result.push_back(std::move(new_i));
    }

    void ASTToHIRVisitor::visit(ast::InstructionReturn *i) {
        i->getValue()->accept(this);
        auto value = std::move(last_expr);
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
        auto cond = std::move(last_expr);
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
        auto cond = std::move(last_expr);
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
        auto var_prev = resolveUseScope(e->getAtom().get());
        last_expr = std::move(var_prev);
    }

    void ASTToHIRVisitor::visit(ast::ParenExpr *e) {
        e->getBody()->accept(this);
    }

    void ASTToHIRVisitor::visit(ast::CallExpr *e) {
        std::vector<std::unique_ptr<Atom>> args;
        for (auto &arg : e->getArguments()) {
            arg->accept(this);
            args.push_back(std::move(last_expr));
        }

        auto var_declare = makeTemp("v");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = copyAtom(var_declare.get());

        auto callee =
            createUnscopedIdentifier(e->getCallee()->toString(*old_table));
        auto type = function_type_mappings[callee->getValue()]->clone();

        var_type_mappings[var_declare->getValue()] = type.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            std::move(type), std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionCallAssign>(
            std::move(var_assign), std::move(callee), std::move(args));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::UnaryOpExpr *e) {
        e->getValue()->accept(this);
        auto value = std::move(last_expr);
        auto var_declare = makeTemp("v");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = copyAtom(var_declare.get());

        // TODO: figure out the details for type + casting/coercion
        std::unique_ptr<Type> type;
        if (!value->isIdentifier()) {
            // TODO: more types
            type = std::make_unique<Int>();
        } else {
            type = var_type_mappings[value->getValue()]->clone();
        }

        switch (e->getOp()) {
        case UnaryOp::PLUS:
        case UnaryOp::MINUS:
        case UnaryOp::NOT:
            type = std::make_unique<Int>();
            break;
        case UnaryOp::ADDRESS:
            type = std::make_unique<Ptr>(std::move(type));
            break;
        case UnaryOp::DEREF:
            auto ptr = dynamic_cast<Ptr *>(type.get());
            type = std::move(ptr->getBase());
            break;
        }

        var_type_mappings[var_declare->getValue()] = type.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            std::move(type), std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionAssignUnaryOp>(
            std::move(var_assign), e->getOp(), std::move(value));
        result.push_back(std::move(new_assign));
    }

    void ASTToHIRVisitor::visit(ast::BinaryOpExpr *e) {
        e->getLeft()->accept(this);
        auto left = std::move(last_expr);

        e->getRight()->accept(this);
        auto right = std::move(last_expr);

        auto var_declare = makeTemp("v");
        auto var_assign =
            std::make_unique<AtomIdentifier>(var_declare->getValue());
        last_expr = copyAtom(var_declare.get());

        // TODO: figure out the details for type + casting/coercion
        std::unique_ptr<Type> type;
        if (!left->isIdentifier()) {
            // TODO: more types
            type = std::make_unique<Int>();
        } else {
            type = var_type_mappings[left->getValue()]->clone();
        }

        var_type_mappings[var_declare->getValue()] = type.get();

        auto new_declare = std::make_unique<hir::InstructionDeclaration>(
            std::move(type), std::move(var_declare));
        result.push_back(std::move(new_declare));

        auto new_assign = std::make_unique<hir::InstructionAssignBinaryOp>(
            std::move(var_assign), e->getOp(), std::move(left),
            std::move(right));
        result.push_back(std::move(new_assign));
    }

    hir::Program astToHir(ast::Program &ast) {
        auto &old_table = ast.getSymbolTable();
        auto new_table = std::make_unique<SymbolTable>();

        std::vector<std::unique_ptr<hir::Function>> functions;
        ASTToHIRVisitor visitor(old_table.get(), new_table.get());
        for (auto &f : ast.getFunctions()) {
            // TODO: deduplicate somehow (visitor or something else)
            auto prototype = dynamic_cast<ast::FunctionPrototype *>(f.get());
            if (prototype) {
                auto type = std::move(prototype->getType());
                auto name = visitor.createUnscopedIdentifier(
                    prototype->getName()->toString(*old_table));
                visitor.mapFunctionType(name.get(), type.get());

                std::vector<hir::Parameter> params;
                for (auto &[param_type, param_name] :
                     prototype->getParameters()) {
                    auto param =
                        visitor.resolveDeclarationScope(param_name.get());
                    visitor.mapVariableType(param.get(), param_type.get());
                    params.push_back({std::move(param_type), std::move(param)});
                }

                auto hir_prototype = std::make_unique<hir::FunctionPrototype>(
                    std::move(type), std::move(name), std::move(params));

                functions.push_back(std::move(hir_prototype));
                visitor.clearResult();
            }

            auto function = dynamic_cast<ast::FunctionDefinition *>(f.get());
            if (function) {
                auto type = std::move(function->getType());
                auto name = visitor.createUnscopedIdentifier(
                    function->getName()->toString(*old_table));
                visitor.mapFunctionType(name.get(), type.get());

                std::vector<hir::Parameter> params;
                for (auto &[param_type, param_name] :
                     function->getParameters()) {
                    auto param =
                        visitor.resolveDeclarationScope(param_name.get());
                    visitor.mapVariableType(param.get(), param_type.get());
                    params.push_back({std::move(param_type), std::move(param)});
                }

                function->getBody()->accept(&visitor);
                visitor.addReturnIfMissing(function);
                auto body = visitor.getResult();

                auto hir_function = std::make_unique<hir::FunctionDefinition>(
                    std::move(type), std::move(name), std::move(params),
                    std::move(body));

                functions.push_back(std::move(hir_function));
                visitor.clearResult();
            }
        }

        hir::Program hir_program(std::move(functions), std::move(new_table));
        return std::move(hir_program);
    }
} // namespace frontend
