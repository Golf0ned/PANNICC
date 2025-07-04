#pragma once

#include <vector>
#include <string>

#include "frontend/ast/atom.h"
#include "frontend/ast/instruction.h"


namespace frontend::ast {
    class Program {
        public:
            void addInstruction(Instruction* i);
            std::string toString();
            ~Program();
        private:
            std::vector<Instruction*> instructions;
    };

    class ToStringVisitor : public InstructionVisitor, public AtomVisitor {
        public:
            ToStringVisitor();
            std::string getResult();

            void visit(Atom* a) override;
            void visit(AtomIdentifier* a) override;
            void visit(AtomLiteral* a) override;

            void visit(Instruction* i) override;
            void visit(InstructionDeclaration* i) override;
            void visit(InstructionAssignValue* i) override;
            void visit(InstructionAssignBinaryOp* i) override;
            void visit(InstructionReturn* i) override;

        private:
            std::string res;
    };
}
