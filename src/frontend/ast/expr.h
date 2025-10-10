#pragma once

#include <memory>

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"

namespace frontend::ast {
    class Expr {};

    class TerminalExpr : public Expr {
    public:
        TerminalExpr(std::unique_ptr<Atom> atom);

    private:
        std::unique_ptr<Atom> atom;
    };

    class CallExpr : public Expr {
    public:
        CallExpr(std::unique_ptr<AtomIdentifier> callee);

    private:
        std::unique_ptr<AtomIdentifier> callee;
        // TODO: params
    };

    class UnaryOpExpr : public Expr {
    public:
        UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value);

    private:
        UnaryOp op;
        std::unique_ptr<Expr> value;
    };

    class BinaryOpExpr : public Expr {
    public:
        BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                     std::unique_ptr<Expr> right);

    private:
        BinaryOp op;
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
    };
} // namespace frontend::ast
