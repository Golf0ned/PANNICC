#include "frontend/ast/instruction.h"

using namespace frontend;
using namespace frontend::ast;

void Scope::addInstruction(std::unique_ptr<Instruction> i) {
    instructions.push_back(std::move(i));
}

std::vector<std::unique_ptr<Instruction>> &Scope::getInstructions() {
    return instructions;
}

void Scope::accept(InstructionVisitor *v) { v->visit(this); }

InstructionExpr::InstructionExpr(std::unique_ptr<Expr> expr)
    : expr(std::move(expr)) {}

Expr *InstructionExpr::getExpr() { return expr.get(); }

void InstructionExpr::accept(InstructionVisitor *v) { v->visit(this); }

InstructionDeclaration::InstructionDeclaration(
    std::unique_ptr<Type> type, std::unique_ptr<AtomIdentifier> variable)
    : type(std::move(type)), variable(std::move(variable)), value(nullptr) {}

InstructionDeclaration::InstructionDeclaration(
    std::unique_ptr<Type> type, std::unique_ptr<AtomIdentifier> variable,
    std::unique_ptr<Expr> value)
    : type(std::move(type)), variable(std::move(variable)),
      value(std::move(value)) {}

Type *InstructionDeclaration::getType() { return type.get(); }

AtomIdentifier *InstructionDeclaration::getVariable() { return variable.get(); }

void InstructionDeclaration::accept(InstructionVisitor *v) { v->visit(this); }

InstructionReturn::InstructionReturn(std::unique_ptr<Expr> value)
    : value(std::move(value)) {}

Expr *InstructionReturn::getValue() { return value.get(); }

void InstructionReturn::accept(InstructionVisitor *v) { v->visit(this); }

InstructionIf::InstructionIf(std::unique_ptr<Expr> cond,
                             std::unique_ptr<Instruction> t_branch,
                             std::unique_ptr<Instruction> f_branch)
    : cond(std::move(cond)), t_branch(std::move(t_branch)),
      f_branch(std::move(f_branch)) {}

Expr *InstructionIf::getCond() { return cond.get(); }

Instruction *InstructionIf::getTBranch() { return t_branch.get(); }

Instruction *InstructionIf::getFBranch() { return f_branch.get(); }

bool InstructionIf::hasFBranch() { return f_branch != nullptr; }

void InstructionIf::accept(InstructionVisitor *v) { v->visit(this); }

InstructionWhile::InstructionWhile(std::unique_ptr<Expr> cond,
                                   std::unique_ptr<Instruction> body)
    : cond(std::move(cond)), body(std::move(body)) {}

Expr *InstructionWhile::getCond() { return cond.get(); }

Instruction *InstructionWhile::getBody() { return body.get(); }

void InstructionWhile::accept(InstructionVisitor *v) { v->visit(this); }
