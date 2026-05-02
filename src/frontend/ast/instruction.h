#pragma once

#include "frontend/ast/expr.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/type.h"

#include <memory>
#include <vector>

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
        void accept(InstructionVisitor *v) override;

    private:
        std::vector<std::unique_ptr<Instruction>> instructions;
    };

    class InstructionExpr : public Instruction {
    public:
        InstructionExpr(std::unique_ptr<Expr> expr);
        Expr *getExpr();
        void accept(InstructionVisitor *v) override;

    private:
        std::unique_ptr<Expr> expr;
    };

    class InstructionDeclaration : public Instruction {
    public:
        InstructionDeclaration(std::unique_ptr<Type> type,
                               std::unique_ptr<AtomIdentifier> variable);
        InstructionDeclaration(std::unique_ptr<Type> type,
                               std::unique_ptr<AtomIdentifier> variable,
                               std::unique_ptr<Expr> value);
        Type *getType();
        AtomIdentifier *getVariable();
        Expr *getValue();
        void accept(InstructionVisitor *v) override;

    private:
        std::unique_ptr<Type> type;
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<Expr> value;
    };

    class InstructionReturn : public Instruction {
    public:
        InstructionReturn(std::unique_ptr<Expr> value);
        Expr *getValue();
        void accept(InstructionVisitor *v) override;

    private:
        std::unique_ptr<Expr> value;
    };

    class InstructionIf : public Instruction {
    public:
        InstructionIf(std::unique_ptr<Expr> cond,
                      std::unique_ptr<Instruction> t_branch,
                      std::unique_ptr<Instruction> f_branch);
        Expr *getCond();
        Instruction *getTBranch();
        Instruction *getFBranch();
        bool hasFBranch();
        void accept(InstructionVisitor *v) override;

    private:
        std::unique_ptr<Expr> cond;
        std::unique_ptr<Instruction> t_branch;
        std::unique_ptr<Instruction> f_branch;
    };

    class InstructionWhile : public Instruction {
    public:
        InstructionWhile(std::unique_ptr<Expr> cond,
                         std::unique_ptr<Instruction> body);
        Expr *getCond();
        Instruction *getBody();
        void accept(InstructionVisitor *v) override;

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
        virtual void visit(InstructionReturn *i) = 0;
        virtual void visit(InstructionIf *i) = 0;
        virtual void visit(InstructionWhile *i) = 0;
    };
} // namespace frontend::ast
