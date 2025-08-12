#pragma once

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"


namespace frontend::hir {
    class InstructionVisitor;

    class Instruction {
        public:
            virtual void accept(InstructionVisitor* visitor) = 0;
            virtual ~Instruction() = default;
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

    class InstructionCall : public Instruction {
        public:
            InstructionCall(AtomIdentifier* callee);
            AtomIdentifier* getCallee();
            void accept(InstructionVisitor* visitor);
            ~InstructionCall() { delete callee; }

        private:
            AtomIdentifier* callee;
    };

    class InstructionCallAssign : public Instruction {
        public:
            InstructionCallAssign(AtomIdentifier* variable, AtomIdentifier* callee);
            AtomIdentifier* getVariable();
            AtomIdentifier* getCallee();
            void accept(InstructionVisitor* visitor);
            ~InstructionCallAssign() { delete variable; delete callee; }

        private:
            AtomIdentifier* variable;
            AtomIdentifier* callee;
    };

    class InstructionVisitor {
        public:
            virtual void visit(Instruction* i) = 0;
            virtual void visit(InstructionDeclaration* i) = 0;
            virtual void visit(InstructionAssignValue* i) = 0;
            virtual void visit(InstructionAssignBinaryOp* i) = 0;
            virtual void visit(InstructionReturn* i) = 0;
            virtual void visit(InstructionCall* i) = 0;
            virtual void visit(InstructionCallAssign* i) = 0;
    };
}
