#pragma once

#include <vector>
#include <string>


namespace frontend {
    class Visitor;

    class Instruction {
        public:
            virtual void accept(Visitor* visitor) = 0;
    };

    class InstructionDeclaration : public Instruction {
        public:
            void accept(Visitor* visitor);
    };

    class InstructionAssignValue : public Instruction {
        public:
            void accept(Visitor* visitor);
    };

    class InstructionAssignBinaryOp : public Instruction {
        public:
            void accept(Visitor* visitor);
    };

    class InstructionReturn : public Instruction {
        public:
            void accept(Visitor* visitor);
    };

    class AST {
        public:
            // not set on this being public yet
            std::vector<Instruction*> instructions;
            std::string toString();
    };

    class Visitor {
        public:
            virtual void visit(Instruction* i) = 0;
            virtual void visit(InstructionDeclaration* i) = 0;
            virtual void visit(InstructionAssignValue* i) = 0;
            virtual void visit(InstructionAssignBinaryOp* i) = 0;
            virtual void visit(InstructionReturn* i) = 0;
    };

    class ToStringVisitor : public Visitor {
        public:
            ToStringVisitor();
            std::string getResult();
            void visit(Instruction* i) override;
            void visit(InstructionDeclaration* i) override;
            void visit(InstructionAssignValue* i) override;
            void visit(InstructionAssignBinaryOp* i) override;
            void visit(InstructionReturn* i) override;

        private:
            std::string res;
    };
}
