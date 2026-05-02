#include "frontend/ast/expr.h"

using namespace frontend;
using namespace frontend::ast;

TerminalExpr::TerminalExpr(std::unique_ptr<Atom> atom)
    : atom(std::move(atom)) {}

Atom *TerminalExpr::getAtom() { return atom.get(); }

void TerminalExpr::accept(ExprVisitor *v) { v->visit(this); }

ParenExpr::ParenExpr(std::unique_ptr<Expr> body) : body(std::move(body)) {}

Expr *ParenExpr::getBody() { return body.get(); }

void ParenExpr::accept(ExprVisitor *v) { v->visit(this); }

CallExpr::CallExpr(std::unique_ptr<AtomIdentifier> callee,
                   std::vector<std::unique_ptr<Expr>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {}

AtomIdentifier *CallExpr::getCallee() { return callee.get(); }

std::vector<std::unique_ptr<Expr>> &CallExpr::getArguments() {
    return arguments;
}

void CallExpr::accept(ExprVisitor *v) { v->visit(this); }

UnaryOpExpr::UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value)
    : op(op), value(std::move(value)) {}

UnaryOp UnaryOpExpr::getOp() { return op; }

Expr *UnaryOpExpr::getValue() { return value.get(); }

void UnaryOpExpr::accept(ExprVisitor *v) { v->visit(this); }

BinaryOpExpr::BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                           std::unique_ptr<Expr> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

BinaryOp BinaryOpExpr::getOp() { return op; }

Expr *BinaryOpExpr::getLeft() { return left.get(); }

Expr *BinaryOpExpr::getRight() { return right.get(); }

void BinaryOpExpr::accept(ExprVisitor *v) { v->visit(this); }
