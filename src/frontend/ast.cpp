#include <string>

#include "frontend/ast.h"


namespace frontend {
    void Instruction::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    void InstructionDeclaration::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    void InstructionAssignValue::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    void InstructionAssignBinaryOp::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    void InstructionReturn::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    std::string AST::toString() {
        std::string res = "";

        ToStringVisitor visitor = ToStringVisitor();
        for (Instruction* i : instructions) {
            i->accept(&visitor);
        }
        res = visitor.getResult();
        return res;
    }

    ToStringVisitor::ToStringVisitor() : res("") {}

    std::string ToStringVisitor::getResult() {
        return res;
    }

    void ToStringVisitor::visit(Instruction* i) {
        res += "***UNKNOWN INSTRUCTION***\n";
    }

    void ToStringVisitor::visit(InstructionDeclaration* i) {
        res += "Instruction Declaration\n";
    }

    void ToStringVisitor::visit(InstructionAssignValue* i) {
        res += "Instruction Assign Value\n";
    }

    void ToStringVisitor::visit(InstructionAssignBinaryOp* i) {
        res += "Instruction Assign Binary Op\n";
    }

    void ToStringVisitor::visit(InstructionReturn* i) {
        res += "Instruction Return\n";
    }
}
