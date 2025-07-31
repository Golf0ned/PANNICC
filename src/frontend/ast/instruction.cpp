#include "frontend/ast/instruction.h"


namespace frontend::ast {
    void Scope::addInstruction(Instruction* i) {
        instructions.push_back(i);
    }

    std::vector<Instruction*> Scope::getInstructions() {
        return instructions;
    }

    void Scope::accept(InstructionVisitor* visitor) {
        visitor->visit(this);
    }

    Scope::~Scope() {
        for (Instruction* i : instructions) {
            delete i;
        }
    }


    InstructionDeclaration::InstructionDeclaration(Type type, AtomIdentifier* variable)
        : type(type), variable(variable) {}

    Type InstructionDeclaration::getType() {
        return type;
    }

    AtomIdentifier* InstructionDeclaration::getVariable() {
        return variable;
    }

    void InstructionDeclaration::accept(InstructionVisitor* visitor) {
        visitor->visit(this);
    }


    InstructionAssignValue::InstructionAssignValue(AtomIdentifier* variable, Atom* value)
        : variable(variable), value(value) {}

    AtomIdentifier* InstructionAssignValue::getVariable() {
        return variable;
    }

    Atom* InstructionAssignValue::getValue() {
        return value;
    }

    void InstructionAssignValue::accept(InstructionVisitor* visitor) {
        visitor->visit(this);
    }


    InstructionAssignBinaryOp::InstructionAssignBinaryOp(AtomIdentifier* variable, BinaryOp op, Atom* left, Atom* right)
        : variable(variable), op(op), left(left), right(right) {}
    
    AtomIdentifier* InstructionAssignBinaryOp::getVariable() {
        return variable;
    }

    BinaryOp InstructionAssignBinaryOp::getOp() {
        return op;
    }

    Atom* InstructionAssignBinaryOp::getRight() {
        return right;
    }

    Atom* InstructionAssignBinaryOp::getLeft() {
        return left;
    }

    void InstructionAssignBinaryOp::accept(InstructionVisitor* visitor) {
        visitor->visit(this);
    }


    InstructionReturn::InstructionReturn(Atom* value) : value(value) {}

    Atom* InstructionReturn::getValue() {
        return value;
    }

    void InstructionReturn::accept(InstructionVisitor* visitor) {
        visitor->visit(this);
    }
}
