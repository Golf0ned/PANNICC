#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"

namespace frontend::hir {
    Label::Label(std::unique_ptr<AtomIdentifier> name)
        : name(std::move(name)) {}

    std::unique_ptr<AtomIdentifier> &Label::getName() { return name; }

    void Label::accept(InstructionVisitor *visitor) { visitor->visit(this); }

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

    InstructionAssignUnaryOp::InstructionAssignUnaryOp(
        std::unique_ptr<AtomIdentifier> variable, UnaryOp op,
        std::unique_ptr<Atom> value)
        : variable(std::move(variable)), op(op), value(std::move(value)) {}

    std::unique_ptr<AtomIdentifier> &InstructionAssignUnaryOp::getVariable() {
        return variable;
    }

    UnaryOp InstructionAssignUnaryOp::getOp() { return op; }

    std::unique_ptr<Atom> &InstructionAssignUnaryOp::getValue() {
        return value;
    }

    void InstructionAssignUnaryOp::accept(InstructionVisitor *visitor) {
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

    InstructionCall::InstructionCall(
        std::unique_ptr<AtomIdentifier> callee,
        std::vector<std::unique_ptr<Atom>> arguments)
        : callee(std::move(callee)), arguments(std::move(arguments)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCall::getCallee() {
        return callee;
    }

    std::vector<std::unique_ptr<Atom>> &InstructionCall::getArguments() {
        return arguments;
    }

    void InstructionCall::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionCallAssign::InstructionCallAssign(
        std::unique_ptr<AtomIdentifier> variable,
        std::unique_ptr<AtomIdentifier> callee,
        std::vector<std::unique_ptr<Atom>> arguments)
        : variable(std::move(variable)), callee(std::move(callee)),
          arguments(std::move(arguments)) {}

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getVariable() {
        return variable;
    }

    std::unique_ptr<AtomIdentifier> &InstructionCallAssign::getCallee() {
        return callee;
    }

    std::vector<std::unique_ptr<Atom>> &InstructionCallAssign::getArguments() {
        return arguments;
    }

    void InstructionCallAssign::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionBranch::InstructionBranch(std::unique_ptr<AtomIdentifier> label)
        : label(std::move(label)) {}

    std::unique_ptr<AtomIdentifier> &InstructionBranch::getLabel() {
        return label;
    }

    void InstructionBranch::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }

    InstructionBranchCond::InstructionBranchCond(
        std::unique_ptr<Atom> cmp, std::unique_ptr<AtomIdentifier> t_label,
        std::unique_ptr<AtomIdentifier> f_label)
        : cmp(std::move(cmp)), t_label(std::move(t_label)),
          f_label(std::move(f_label)) {}

    std::unique_ptr<Atom> &InstructionBranchCond::getCmp() { return cmp; }

    std::unique_ptr<AtomIdentifier> &InstructionBranchCond::getTLabel() {
        return t_label;
    }

    std::unique_ptr<AtomIdentifier> &InstructionBranchCond::getFLabel() {
        return f_label;
    }

    void InstructionBranchCond::accept(InstructionVisitor *visitor) {
        visitor->visit(this);
    }
} // namespace frontend::hir
