#include "frontend/ast/expr.h"

namespace frontend::ast {
    TerminalExpr::TerminalExpr(std::unique_ptr<Atom> atom)
        : atom(std::move(atom)) {}

    std::unique_ptr<Atom> &TerminalExpr::getAtom() { return atom; }

    void TerminalExpr::accept(ExprVisitor *visitor) { visitor->visit(this); }

    ParenExpr::ParenExpr(std::unique_ptr<Expr> body) : body(std::move(body)) {}

    std::unique_ptr<Expr> &ParenExpr::getBody() { return body; }

    void ParenExpr::accept(ExprVisitor *visitor) { visitor->visit(this); }

    CallExpr::CallExpr(std::unique_ptr<AtomIdentifier> callee)
        : callee(std::move(callee)) {}

    std::unique_ptr<AtomIdentifier> &CallExpr::getCallee() { return callee; }

    void CallExpr::accept(ExprVisitor *visitor) { visitor->visit(this); }

    UnaryOpExpr::UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value)
        : op(op), value(std::move(value)) {}

    UnaryOp UnaryOpExpr::getOp() { return op; }

    std::unique_ptr<Expr> &UnaryOpExpr::getValue() { return value; }

    void UnaryOpExpr::accept(ExprVisitor *visitor) { visitor->visit(this); }

    BinaryOpExpr::BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                               std::unique_ptr<Expr> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    BinaryOp BinaryOpExpr::getOp() { return op; }

    std::unique_ptr<Expr> &BinaryOpExpr::getLeft() { return left; }

    std::unique_ptr<Expr> &BinaryOpExpr::getRight() { return right; }

    void BinaryOpExpr::accept(ExprVisitor *visitor) { visitor->visit(this); }
} // namespace frontend::ast
