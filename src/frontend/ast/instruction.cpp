#include "frontend/ast/instruction.h"

namespace frontend::ast {
    void Scope::addInstruction(std::unique_ptr<Instruction> i) {
        instructions.push_back(std::move(i));
    }

    std::vector<std::unique_ptr<Instruction>> &Scope::getInstructions() {
        return instructions;
    }

    void Scope::accept(InstructionVisitor *visitor) { visitor->visit(this); }

    InstructionExpr::InstructionExpr(std::unique_ptr<Expr> expr)
        : expr(std::move(expr)) {}

    std::unique_ptr<Expr> &InstructionExpr::getExpr() { return expr; }

    void InstructionExpr::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionDeclaration::InstructionDeclaration(
        Type type, std::unique_ptr<AtomIdentifier> variable)
        : type(type), variable(std::move(variable)) {}

    Type InstructionDeclaration::getType() { return type; }

    std::unique_ptr<AtomIdentifier> &InstructionDeclaration::getVariable() {
        return variable;
    }

    void InstructionDeclaration::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionDeclarationAssign::InstructionDeclarationAssign(
        Type type, std::unique_ptr<AtomIdentifier> variable,
        std::unique_ptr<Expr> value)
        : type(type), variable(std::move(variable)), value(std::move(value)) {}

    Type InstructionDeclarationAssign::getType() { return type; }

    std::unique_ptr<AtomIdentifier> &
    InstructionDeclarationAssign::getVariable() {
        return variable;
    }

    std::unique_ptr<Expr> &InstructionDeclarationAssign::getValue() {
        return value;
    }

    void InstructionDeclarationAssign::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionAssign::InstructionAssign(
        std::unique_ptr<AtomIdentifier> variable, std::unique_ptr<Expr> value)
        : variable(std::move(variable)), value(std::move(value)) {}

    std::unique_ptr<AtomIdentifier> &InstructionAssign::getVariable() {
        return variable;
    }

    std::unique_ptr<Expr> &InstructionAssign::getValue() { return value; }

    void InstructionAssign::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionReturn::InstructionReturn(std::unique_ptr<Expr> value)
        : value(std::move(value)) {}

    std::unique_ptr<Expr> &InstructionReturn::getValue() { return value; }

    void InstructionReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionIf::InstructionIf(std::unique_ptr<Expr> cond,
                                 std::unique_ptr<Instruction> t_branch,
                                 std::unique_ptr<Instruction> f_branch)
        : cond(std::move(cond)), t_branch(std::move(t_branch)),
          f_branch(std::move(f_branch)) {}

    std::unique_ptr<Expr> &InstructionIf::getCond() { return cond; }

    std::unique_ptr<Instruction> &InstructionIf::getTBranch() {
        return t_branch;
    }

    std::unique_ptr<Instruction> &InstructionIf::getFBranch() {
        return f_branch;
    }

    bool InstructionIf::hasFBranch() { return f_branch != nullptr; }

    void InstructionIf::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionWhile::InstructionWhile(std::unique_ptr<Expr> cond,
                                       std::unique_ptr<Instruction> body)
        : cond(std::move(cond)), body(std::move(body)) {}

    std::unique_ptr<Expr> &InstructionWhile::getCond() { return cond; }

    std::unique_ptr<Instruction> &InstructionWhile::getBody() { return body; }

    void InstructionWhile::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace frontend::ast
