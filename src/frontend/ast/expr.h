#pragma once

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"

#include <memory>
#include <vector>

namespace frontend::ast {

class ExprVisitor;

class Expr {
public:
    virtual void accept(ExprVisitor *v) = 0;
    virtual ~Expr() = default;
};

class TerminalExpr : public Expr {
public:
    TerminalExpr(std::unique_ptr<Atom> atom);
    Atom *getAtom();
    void accept(ExprVisitor *v) override;

private:
    std::unique_ptr<Atom> atom;
};

class ParenExpr : public Expr {
public:
    ParenExpr(std::unique_ptr<Expr> body);
    Expr *getBody();
    void accept(ExprVisitor *v) override;

private:
    std::unique_ptr<Expr> body;
};

class CallExpr : public Expr {
public:
    CallExpr(std::unique_ptr<AtomIdentifier> callee,
             std::vector<std::unique_ptr<Expr>> arguments);
    AtomIdentifier *getCallee();
    std::vector<std::unique_ptr<Expr>> &getArguments();
    void accept(ExprVisitor *v) override;

private:
    std::unique_ptr<AtomIdentifier> callee;
    std::vector<std::unique_ptr<Expr>> arguments;
};

class UnaryOpExpr : public Expr {
public:
    UnaryOpExpr(UnaryOp op, std::unique_ptr<Expr> value);
    UnaryOp getOp();
    Expr *getValue();
    void accept(ExprVisitor *v) override;

private:
    UnaryOp op;
    std::unique_ptr<Expr> value;
};

class BinaryOpExpr : public Expr {
public:
    BinaryOpExpr(BinaryOp op, std::unique_ptr<Expr> left,
                 std::unique_ptr<Expr> right);
    BinaryOp getOp();
    Expr *getLeft();
    Expr *getRight();
    void accept(ExprVisitor *v) override;

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
