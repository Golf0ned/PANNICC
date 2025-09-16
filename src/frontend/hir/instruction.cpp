#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"

namespace frontend::hir {
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

    InstructionCall::InstructionCall(std::unique_ptr<AtomIdentifier> callee)
        : callee(std::move(callee)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCall::getCallee() {
        return callee;
    }

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCallAssign::InstructionCallAssign(
        std::unique_ptr<AtomIdentifier> variable,
        std::unique_ptr<AtomIdentifier> callee)
        : variable(std::move(variable)), callee(std::move(callee)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getVariable() {
        return variable;
    }

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getCallee() {
        return callee;
    }

    void InstructionCallAssign::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace frontend::hir
