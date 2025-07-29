#pragma once

#include <vector>
#include <string>

#include "frontend/ast/atom.h"
#include "frontend/ast/instruction.h"
#include "frontend/type.h"


namespace frontend::ast {
    class Function {
        public:
            Function(Type type, AtomIdentifier* name, Scope* body);
            Type getType();
            AtomIdentifier* getName();
            Scope* getBody();
            std::string toString(std::unordered_map<uint64_t, std::string>& symbol_table);
        private:
            Type type;
            AtomIdentifier* name;
            // TODO: params
            Scope* body;
    };

    class Program {
        public:
            void addFunction(Function f);
            void addSymbol(uint64_t id, const std::string& symbol);
            std::vector<Function>& getFunctions();
            std::string toString();
            ~Program();
        private:
            std::vector<Function> functions;
            std::unordered_map<uint64_t, std::string> symbol_table;
    };

    class ToStringVisitor : public InstructionVisitor, public AtomVisitor {
        public:
            ToStringVisitor(std::unordered_map<uint64_t, std::string>& symbol_table);
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
            std::unordered_map<uint64_t, std::string>& symbol_table;
            std::string prefix;
            std::string res;
    };
}
