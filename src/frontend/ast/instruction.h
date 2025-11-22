#pragma once

#include <memory>
#include <vector>

#include "frontend/ast/expr.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *v) = 0;
        virtual ~Instruction() = default;
    };

    class Scope : public Instruction {
    public:
        void addInstruction(std::unique_ptr<Instruction> i);
        std::vector<std::unique_ptr<Instruction>> &getInstructions();
        void accept(InstructionVisitor *v);

    private:
        std::vector<std::unique_ptr<Instruction>> instructions;
    };

    class InstructionExpr : public Instruction {
    public:
        InstructionExpr(std::unique_ptr<Expr> expr);
        std::unique_ptr<Expr> &getExpr();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<Expr> expr;
    };

    class InstructionDeclaration : public Instruction {
    public:
        InstructionDeclaration(Type type,
                               std::unique_ptr<AtomIdentifier> variable);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getVariable();
        void accept(InstructionVisitor *v);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> variable;
    };

    class InstructionDeclarationAssign : public Instruction {
    public:
        InstructionDeclarationAssign(Type type,
                                     std::unique_ptr<AtomIdentifier> variable,
                                     std::unique_ptr<Expr> value);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<Expr> &getValue();
        void accept(InstructionVisitor *v);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<Expr> value;
    };

    class InstructionAssign : public Instruction {
    public:
        InstructionAssign(std::unique_ptr<AtomIdentifier> variable,
                          std::unique_ptr<Expr> value);
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<Expr> &getValue();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<Expr> value;
    };

    class InstructionOpAssign : public Instruction {
    public:
        InstructionOpAssign(std::unique_ptr<AtomIdentifier> variable,
                            BinaryOp op, std::unique_ptr<Expr> value);
        std::unique_ptr<AtomIdentifier> &getVariable();
        BinaryOp getOp();
        std::unique_ptr<Expr> &getValue();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        BinaryOp op;
        std::unique_ptr<Expr> value;
    };

    class InstructionReturn : public Instruction {
    public:
        InstructionReturn(std::unique_ptr<Expr> value);
        std::unique_ptr<Expr> &getValue();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<Expr> value;
    };

    class InstructionIf : public Instruction {
    public:
        InstructionIf(std::unique_ptr<Expr> cond,
                      std::unique_ptr<Instruction> t_branch,
                      std::unique_ptr<Instruction> f_branch);
        std::unique_ptr<Expr> &getCond();
        std::unique_ptr<Instruction> &getTBranch();
        std::unique_ptr<Instruction> &getFBranch();
        bool hasFBranch();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Instruction> t_branch;
        std::unique_ptr<Instruction> f_branch;
    };

    class InstructionWhile : public Instruction {
    public:
        InstructionWhile(std::unique_ptr<Expr> cond,
                         std::unique_ptr<Instruction> body);
        std::unique_ptr<Expr> &getCond();
        std::unique_ptr<Instruction> &getBody();
        void accept(InstructionVisitor *v);

    private:
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Instruction> body;
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(Scope *s) = 0;
        virtual void visit(InstructionExpr *i) = 0;
        virtual void visit(InstructionDeclaration *i) = 0;
        virtual void visit(InstructionDeclarationAssign *i) = 0;
        virtual void visit(InstructionAssign *i) = 0;
        virtual void visit(InstructionOpAssign *i) = 0;
        virtual void visit(InstructionReturn *i) = 0;
        virtual void visit(InstructionIf *i) = 0;
        virtual void visit(InstructionWhile *i) = 0;
    };
} // namespace frontend::ast
