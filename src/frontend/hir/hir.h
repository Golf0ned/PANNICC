#pragma once

#include <string>
#include <vector>

#include "frontend/hir/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend::hir {
    class Function {
        public:
            Function(Type type, AtomIdentifier *name,
                     std::vector<Instruction *> body);
            Type getType();
            AtomIdentifier *getName();
            std::vector<Instruction *> getBody();
            std::string toString(SymbolTable &symbol_table);

        private:
            Type type;
            AtomIdentifier *name;
            // TODO: params
            std::vector<Instruction *> body;
    };

    class Program {
        public:
            Program(std::vector<Function> functions, SymbolTable &symbol_table);
            std::vector<Function> &getFunctions();
            SymbolTable &getSymbolTable();
            std::string toString();
            ~Program();

        private:
            std::vector<Function> functions;
            SymbolTable symbol_table;
    };

    class ToStringVisitor : public InstructionVisitor {
        public:
            ToStringVisitor(SymbolTable &symbol_table);
            std::string getResult();

            void visit(Instruction *i) override;
            void visit(InstructionDeclaration *i) override;
            void visit(InstructionAssignValue *i) override;
            void visit(InstructionAssignBinaryOp *i) override;
            void visit(InstructionReturn *i) override;
            void visit(InstructionCall *i) override;
            void visit(InstructionCallAssign *i) override;

        private:
            SymbolTable &symbol_table;
            std::string prefix;
            std::string res;
    };
} // namespace frontend::hir
