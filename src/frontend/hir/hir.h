#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include "frontend/hir/atom.h"
#include "frontend/hir/instruction.h"


namespace frontend::hir {
    class Function {
        public:
            Function(Type type, AtomIdentifier* name, std::vector<Instruction*> body);
            Type getType();
            AtomIdentifier* getName();
            std::vector<Instruction*> getBody();
            std::string toString(std::unordered_map<uint64_t, std::string>& symbol_table);
        private:
            Type type;
            AtomIdentifier* name;
            // TODO: params
            std::vector<Instruction*> body;
    };

    class Program {
        public:
            // TODO: remove
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
