#pragma once

#include <string>
#include <vector>

#include "frontend/ast/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    class Function {
    public:
        Function(Type type, std::unique_ptr<AtomIdentifier> name,
                 std::unique_ptr<Scope> body);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getName();
        std::unique_ptr<Scope> &getBody();
        std::string toString(SymbolTable *symbol_table);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> name;
        // TODO: params
        std::unique_ptr<Scope> body;
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

    class ToStringVisitor : public InstructionVisitor, public ExprVisitor {
    public:
        ToStringVisitor(SymbolTable *symbol_table);
        std::string getResult();

        void convertSubexpression(Instruction *i);

        void visit(Instruction *i) override;
        void visit(Scope *s) override;
        void visit(InstructionExpr *i) override;
        void visit(InstructionDeclaration *i) override;
        void visit(InstructionDeclarationAssign *i) override;
        void visit(InstructionAssign *i) override;
        void visit(InstructionReturn *i) override;
        void visit(InstructionIf *i) override;
        void visit(InstructionWhile *i) override;

        void visit(Expr *e) override;
        void visit(TerminalExpr *e) override;
        void visit(ParenExpr *e) override;
        void visit(CallExpr *e) override;
        void visit(UnaryOpExpr *e) override;
        void visit(BinaryOpExpr *e) override;

    private:
        SymbolTable *symbol_table;
        std::string prefix;
        std::string res;
    };
} // namespace frontend::ast
