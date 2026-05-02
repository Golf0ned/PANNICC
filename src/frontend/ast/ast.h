#pragma once

#include "frontend/ast/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

#include <string>
#include <vector>

namespace frontend::ast {
    using Parameter =
        std::pair<std::unique_ptr<Type>, std::unique_ptr<AtomIdentifier>>;

    class Function {
    public:
        virtual std::string toString(SymbolTable *st) = 0;
        virtual ~Function() = default;
    };

    class FunctionDefinition : public Function {
    public:
        FunctionDefinition(std::unique_ptr<Type> type,
                           std::unique_ptr<AtomIdentifier> name,
                           std::vector<Parameter> parameters,
                           std::unique_ptr<Scope> body);
        Type *getType();
        AtomIdentifier *getName();
        std::vector<Parameter> &getParameters();
        Scope *getBody();
        std::string toString(SymbolTable *st) override;

    private:
        std::unique_ptr<Type> type;
        std::unique_ptr<AtomIdentifier> name;
        std::vector<Parameter> parameters;
        std::unique_ptr<Scope> body;
    };

    class FunctionPrototype : public Function {
    public:
        FunctionPrototype(std::unique_ptr<Type> type,
                          std::unique_ptr<AtomIdentifier> name,
                          std::vector<Parameter> parameters);
        Type *getType();
        AtomIdentifier *getName();
        std::vector<Parameter> &getParameters();
        std::string toString(SymbolTable *st) override;

    private:
        std::unique_ptr<Type> type;
        std::unique_ptr<AtomIdentifier> name;
        std::vector<Parameter> parameters;
    };

    class Program {
    public:
        Program(std::vector<std::unique_ptr<Function>> functions,
                std::unique_ptr<SymbolTable> st);
        std::vector<std::unique_ptr<Function>> &getFunctions();
        SymbolTable *getSymbolTable();
        std::string toString();

    private:
        std::vector<std::unique_ptr<Function>> functions;
        std::unique_ptr<SymbolTable> st;
    };

    class ToStringVisitor : public InstructionVisitor, public ExprVisitor {
    public:
        ToStringVisitor(SymbolTable *st);
        std::string getResult();

        void convertSubexpression(Instruction *i);

        void visit(Instruction *i) override;
        void visit(Scope *s) override;
        void visit(InstructionExpr *i) override;
        void visit(InstructionDeclaration *i) override;
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
        SymbolTable *st;
        std::string prefix;
        std::string res;
    };
} // namespace frontend::ast
