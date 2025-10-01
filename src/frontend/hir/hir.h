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
        Function(Type type, std::unique_ptr<AtomIdentifier> name,
                 std::vector<std::unique_ptr<Instruction>> body);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getName();
        std::vector<std::unique_ptr<Instruction>> &getBody();
        std::string toString(SymbolTable *symbol_table);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> name;
        // TODO: params
        std::vector<std::unique_ptr<Instruction>> body;
    };

    class Program {
    public:
        Program(std::vector<Function> functions,
                std::unique_ptr<SymbolTable> symbol_table);
        std::vector<Function> &getFunctions();
        std::unique_ptr<SymbolTable> &getSymbolTable();
        std::string toString();

    private:
        std::vector<Function> functions;
        std::unique_ptr<SymbolTable> symbol_table;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        ToStringVisitor(SymbolTable *symbol_table);
        std::string getResult();

        void visit(Instruction *i) override;
        void visit(Label *l) override;
        void visit(InstructionDeclaration *i) override;
        void visit(InstructionAssignValue *i) override;
        void visit(InstructionAssignBinaryOp *i) override;
        void visit(InstructionReturn *i) override;
        void visit(InstructionCall *i) override;
        void visit(InstructionCallAssign *i) override;
        void visit(InstructionBranch *i) override;
        void visit(InstructionBranchCond *i) override;

    private:
        SymbolTable *symbol_table;
        std::string res;
    };
} // namespace frontend::hir
