#pragma once

#include <vector>

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"


namespace frontend::ast {
    class InstructionVisitor;

    class Instruction {
        public:
            virtual void accept(InstructionVisitor* visitor) = 0;
            virtual ~Instruction() = default;
    };

    class Scope : public Instruction {
        public:
            void addInstruction(Instruction* i);
            std::vector<Instruction*> getInstructions();
            void accept(InstructionVisitor* visitor);
            ~Scope();
        private:
            std::vector<Instruction*> instructions;
    };

    class InstructionDeclaration : public Instruction {
        public:
            InstructionDeclaration(Type type, AtomIdentifier* variable);
            Type getType();
            AtomIdentifier* getVariable();
            void accept(InstructionVisitor* visitor);
            ~InstructionDeclaration() { delete variable; }

        private:
            Type type;
            AtomIdentifier* variable;
    };

    class InstructionDeclarationAssignValue : public Instruction {
        public:
            InstructionDeclarationAssignValue(Type type, AtomIdentifier* variable, Atom* value);
            Type getType();
            AtomIdentifier* getVariable();
            Atom* getValue();
            void accept(InstructionVisitor* visitor);
            ~InstructionDeclarationAssignValue() { delete variable; delete value; }

        private:
            Type type;
            AtomIdentifier* variable;
            Atom* value;
    };

    class InstructionAssignValue : public Instruction {
        public:
            InstructionAssignValue(AtomIdentifier* variable, Atom* value);
            AtomIdentifier* getVariable();
            Atom* getValue();
            void accept(InstructionVisitor* visitor);
            ~InstructionAssignValue() { delete variable; delete value; }

        private:
            AtomIdentifier* variable;
            Atom* value;
    };

    class InstructionAssignBinaryOp : public Instruction {
        public:
            InstructionAssignBinaryOp(AtomIdentifier* variable, BinaryOp op, Atom* left, Atom* right);
            AtomIdentifier* getVariable();
            BinaryOp getOp();
            Atom* getLeft();
            Atom* getRight();
            void accept(InstructionVisitor* visitor);
            ~InstructionAssignBinaryOp() { delete variable; delete left; delete right; }

        private:
            AtomIdentifier* variable;
            BinaryOp op;
            Atom* left;
            Atom* right;
    };

    class InstructionReturn : public Instruction {
        public:
            InstructionReturn(Atom* value);
            Atom* getValue();
            void accept(InstructionVisitor* visitor);
            ~InstructionReturn() { delete value; }

        private:
            Atom* value;
    };

    class InstructionVisitor {
        public:
            virtual void visit(Instruction* i) = 0;
            virtual void visit(Scope* s) = 0;
            virtual void visit(InstructionDeclaration* i) = 0;
            virtual void visit(InstructionDeclarationAssignValue* i) = 0;
            virtual void visit(InstructionAssignValue* i) = 0;
            virtual void visit(InstructionAssignBinaryOp* i) = 0;
            virtual void visit(InstructionReturn* i) = 0;
    };
}
