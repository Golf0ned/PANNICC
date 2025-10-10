#include "frontend/ast/expr.h"

namespace frontend::ast {
    TerminalExpr::TerminalExpr(std::unique_ptr<Atom> atom)
        : atom(std::move(atom)) {}

    CallExpr::CallExpr(std::unique_ptr<AtomIdentifier> callee)
        : callee(std::move(callee)) {}

    UnaryOpExpr::UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value)
        : op(op), value(std::move(value)) {}

    BinaryOpExpr::BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                               std::unique_ptr<Expr> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
} // namespace frontend::ast
