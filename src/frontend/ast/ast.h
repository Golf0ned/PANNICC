#pragma once

#include "frontend/ast/instruction.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

#include <string>
#include <vector>

namespace frontend::ast {

class FunctionVisitor;

class Function {
public:
    virtual std::string toString(SymbolTable &st) = 0;
    virtual void accept(FunctionVisitor *v) = 0;
    virtual ~Function() = default;
};

using Parameter =
    std::pair<std::unique_ptr<Type>, std::unique_ptr<AtomIdentifier>>;

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
    std::string toString(SymbolTable &st) override;
    void accept(FunctionVisitor *v) override;

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
    std::string toString(SymbolTable &st) override;
    void accept(FunctionVisitor *v) override;

private:
    std::unique_ptr<Type> type;
    std::unique_ptr<AtomIdentifier> name;
    std::vector<Parameter> parameters;
};

class FunctionVisitor {
public:
    virtual void visit(FunctionDefinition *f) = 0;
    virtual void visit(FunctionPrototype *f) = 0;
};

class Program {
public:
    Program(std::vector<std::unique_ptr<Function>> functions, SymbolTable st);
    std::vector<std::unique_ptr<Function>> &getFunctions();
    SymbolTable &getSymbolTable();
    std::string toString();

private:
    std::vector<std::unique_ptr<Function>> functions;
    SymbolTable st;
};

class ToStringVisitor : public InstructionVisitor, public ExprVisitor {
public:
    ToStringVisitor(SymbolTable &st);

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
    SymbolTable &st;
    std::string prefix;
    std::string res;
};

} // namespace frontend::ast
