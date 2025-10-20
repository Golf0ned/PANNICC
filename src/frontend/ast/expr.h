#pragma once

#include <memory>
#include <vector>

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"

namespace frontend::ast {
    class ExprVisitor;

    class Expr {
    public:
        virtual void accept(ExprVisitor *visitor) = 0;
        virtual ~Expr() = default;
    };

    class TerminalExpr : public Expr {
    public:
        TerminalExpr(std::unique_ptr<Atom> atom);
        std::unique_ptr<Atom> &getAtom();
        void accept(ExprVisitor *visitor);

    private:
        std::unique_ptr<Atom> atom;
    };

    class ParenExpr : public Expr {
    public:
        ParenExpr(std::unique_ptr<Expr> body);
        std::unique_ptr<Expr> &getBody();
        void accept(ExprVisitor *visitor);

    private:
        std::unique_ptr<Expr> body;
    };

    class CallExpr : public Expr {
    public:
        CallExpr(std::unique_ptr<AtomIdentifier> callee,
                 std::vector<std::unique_ptr<Expr>> arguments);
        std::unique_ptr<AtomIdentifier> &getCallee();
        std::vector<std::unique_ptr<Expr>> &getArguments();
        void accept(ExprVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> callee;
        std::vector<std::unique_ptr<Expr>> arguments;
    };

    class UnaryOpExpr : public Expr {
    public:
        UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value);
        UnaryOp getOp();
        std::unique_ptr<Expr> &getValue();
        void accept(ExprVisitor *visitor);

    private:
        UnaryOp op;
        std::unique_ptr<Expr> value;
    };

    class BinaryOpExpr : public Expr {
    public:
        BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                     std::unique_ptr<Expr> right);
        BinaryOp getOp();
        std::unique_ptr<Expr> &getLeft();
        std::unique_ptr<Expr> &getRight();
        void accept(ExprVisitor *visitor);

    private:
        BinaryOp op;
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
    };

    class ExprVisitor {
    public:
        virtual void visit(Expr *e) = 0;
        virtual void visit(TerminalExpr *e) = 0;
        virtual void visit(ParenExpr *e) = 0;
        virtual void visit(CallExpr *e) = 0;
        virtual void visit(UnaryOpExpr *e) = 0;
        virtual void visit(BinaryOpExpr *e) = 0;
    };
} // namespace frontend::ast
