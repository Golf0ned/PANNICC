#include "frontend/ast/to_mir.h"

using namespace frontend;

namespace mir = middleend::mir;

mir::Program lower(ast::Program &ast) {
    std::list<std::unique_ptr<mir::Function>> functions;
    mir::LiteralMap lm;

    ToMIRVisitor tmv(ast.getSymbolTable(), lm);
    for (auto &f : ast.getFunctions()) {
        f->accept(&tmv);
        functions.push_back(tmv.getResult()); // might need to move?
    }

    return mir::Program(std::move(functions), std::move(lm));
}

ToMIRVisitor::ToMIRVisitor(SymbolTable &st, mir::LiteralMap &lm)
    : st(st), lm(lm) {};

std::unique_ptr<mir::Function> ToMIRVisitor::getResult() {
    return std::move(res);
}

std::vector<std::unique_ptr<mir::Value>>
convertParams(std::vector<ast::Parameter> &params) {
    std::vector<std::unique_ptr<mir::Value>> res;

    for (auto &[param_type, param_name] : params) {
        // TODO: make variable (with scope?)
        // TODO: make alloca + store, map to param name
        // TODO: map variable type?
        // TODO: add variable to res
    }

    return res;
}

void ToMIRVisitor::resolveBBEdges() {
    // TODO
}

middleend::mir::BasicBlock *createEntryBlock() {
    // TODO
}

void ToMIRVisitor::resolveFunctions() {
    // TODO
}

void ToMIRVisitor::visit(ast::FunctionDefinition *f) {
    auto type = f->getType()->toMir();
    auto name = f->getName()->toString(st);
    auto params = convertParams(f->getParameters());

    // TODO: map function type?
    // TODO: map function in preparation for resolveFunctions

    visit(f->getBody());
    resolveBBEdges();

    auto entry = createEntryBlock();

    res = std::make_unique<mir::FunctionDefinition>(
        type, name, std::move(params), std::move(basic_blocks), entry);
}

void ToMIRVisitor::visit(ast::FunctionPrototype *f) {
    auto type = f->getType()->toMir();
    auto name = f->getName()->toString(st);
    auto params = convertParams(f->getParameters());

    // TODO: map function type?
    // TODO: map function in preparation for resolveFunctions

    res = std::make_unique<mir::FunctionDeclaration>(type, name,
                                                     std::move(params));
}

void ToMIRVisitor::visit(ast::Instruction *i) {}

void ToMIRVisitor::visit(ast::Scope *s) {}

void ToMIRVisitor::visit(ast::InstructionExpr *i) {}

void ToMIRVisitor::visit(ast::InstructionDeclaration *i) {}

void ToMIRVisitor::visit(ast::InstructionReturn *i) {}

void ToMIRVisitor::visit(ast::InstructionIf *i) {}

void ToMIRVisitor::visit(ast::InstructionWhile *i) {}

void ToMIRVisitor::visit(ast::Expr *e) {}

void ToMIRVisitor::visit(ast::TerminalExpr *e) {}

void ToMIRVisitor::visit(ast::ParenExpr *e) {}

void ToMIRVisitor::visit(ast::CallExpr *e) {}

void ToMIRVisitor::visit(ast::UnaryOpExpr *e) {}

void ToMIRVisitor::visit(ast::BinaryOpExpr *e) {}
