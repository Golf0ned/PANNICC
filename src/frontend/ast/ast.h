#pragma once

#include <vector>
#include <string>

#include "frontend/ast/atom.h"
#include "frontend/ast/instruction.h"
#include "frontend/type.h"


namespace frontend::ast {
    class Function {
        public:
            Function(Type type, ast::AtomIdentifier* name, ast::Scope* body);
            Type getType();
            ast::AtomIdentifier* getName();
            ast::Scope* getBody();
            std::string toString();
        private:
            Type type;
            ast::AtomIdentifier* name;
            // TODO: params
            ast::Scope* body;
    };

    class Program {
        public:
            void addFunction(Function f);
            std::string toString();
            ~Program();
        private:
            std::vector<Function> functions;
    };

    class ToStringVisitor : public InstructionVisitor, public AtomVisitor {
        public:
            ToStringVisitor();
            std::string getResult();

            void visit(Atom* a) override;
            void visit(AtomIdentifier* a) override;
            void visit(AtomLiteral* a) override;

            void visit(Instruction* i) override;
            void visit(Scope* s) override;
            void visit(InstructionDeclaration* i) override;
            void visit(InstructionAssignValue* i) override;
            void visit(InstructionAssignBinaryOp* i) override;
            void visit(InstructionReturn* i) override;

        private:
            std::string prefix;
            std::string res;
    };
}
