#include "frontend/ast/instruction.h"

namespace frontend::ast {
    void Scope::addInstruction(std::unique_ptr<Instruction> i) {
        instructions.push_back(std::move(i));
    }

    std::vector<std::unique_ptr<Instruction>> &Scope::getInstructions() {
        return instructions;
    }

    void Scope::accept(InstructionVisitor *visitor) { visitor->visit(this); }

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

    InstructionDeclarationAssignValue::InstructionDeclarationAssignValue(
        Type type, std::unique_ptr<AtomIdentifier> variable,
        std::unique_ptr<Atom> value)
        : type(type), variable(std::move(variable)), value(std::move(value)) {}

    Type InstructionDeclarationAssignValue::getType() { return type; }

    std::unique_ptr<AtomIdentifier> &
    InstructionDeclarationAssignValue::getVariable() {
        return variable;
    }

    std::unique_ptr<Atom> &InstructionDeclarationAssignValue::getValue() {
        return value;
    }

    void
    InstructionDeclarationAssignValue::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionAssignValue::InstructionAssignValue(
        std::unique_ptr<AtomIdentifier> variable, std::unique_ptr<Atom> value)
        : variable(std::move(variable)), value(std::move(value)) {}

    std::unique_ptr<AtomIdentifier> &InstructionAssignValue::getVariable() {
        return variable;
    }

    std::unique_ptr<Atom> &InstructionAssignValue::getValue() { return value; }

    void InstructionAssignValue::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionAssignBinaryOp::InstructionAssignBinaryOp(
        std::unique_ptr<AtomIdentifier> variable, BinaryOp op,
        std::unique_ptr<Atom> left, std::unique_ptr<Atom> right)
        : variable(std::move(variable)), op(op), left(std::move(left)),
          right(std::move(right)) {}

    std::unique_ptr<AtomIdentifier> &InstructionAssignBinaryOp::getVariable() {
        return variable;
    }

    BinaryOp InstructionAssignBinaryOp::getOp() { return op; }

    std::unique_ptr<Atom> &InstructionAssignBinaryOp::getRight() {
        return right;
    }

    std::unique_ptr<Atom> &InstructionAssignBinaryOp::getLeft() { return left; }

    void InstructionAssignBinaryOp::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionReturn::InstructionReturn(std::unique_ptr<Atom> value)
        : value(std::move(value)) {}

    std::unique_ptr<Atom> &InstructionReturn::getValue() { return value; }

    void InstructionReturn::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCall::InstructionCall(std::unique_ptr<AtomIdentifier> target)
        : target(std::move(target)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCall::getTarget() {
        return target;
    }

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCallAssign::InstructionCallAssign(
        std::unique_ptr<AtomIdentifier> variable,
        std::unique_ptr<AtomIdentifier> target)
        : variable(std::move(variable)), target(std::move(target)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getVariable() {
        return variable;
    }

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getTarget() {
        return target;
    }

    void InstructionCallAssign::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace frontend::ast
