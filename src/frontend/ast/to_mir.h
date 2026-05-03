#pragma once

#include "frontend/ast/ast.h"
#include "middleend/mir/mir.h"

namespace frontend {
    middleend::mir::Program lower(ast::Program &ast);

    class ToMIRVisitor : public ast::FunctionVisitor,
                         public ast::InstructionVisitor,
                         public ast::ExprVisitor {
    public:
        ToMIRVisitor(SymbolTable &st, middleend::mir::LiteralMap &literal_map);

        std::unique_ptr<middleend::mir::Function> getResult();

        std::vector<std::unique_ptr<middleend::mir::Value>>
        convertParams(std::vector<ast::Parameter> &params);
        void resolveBBEdges();
        middleend::mir::BasicBlock *createEntryBlock();
        void resolveFunctions();

        void visit(ast::FunctionDefinition *f) override;
        void visit(ast::FunctionPrototype *f) override;

        void visit(ast::Instruction *i) override;
        void visit(ast::Scope *s) override;
        void visit(ast::InstructionExpr *i) override;
        void visit(ast::InstructionDeclaration *i) override;
        void visit(ast::InstructionReturn *i) override;
        void visit(ast::InstructionIf *i) override;
        void visit(ast::InstructionWhile *i) override;

        void visit(ast::Expr *e) override;
        void visit(ast::TerminalExpr *e) override;
        void visit(ast::ParenExpr *e) override;
        void visit(ast::CallExpr *e) override;
        void visit(ast::UnaryOpExpr *e) override;
        void visit(ast::BinaryOpExpr *e) override;

    private:
        std::unique_ptr<middleend::mir::Function> res;
        SymbolTable &st;
        middleend::mir::LiteralMap &lm;
        uint64_t cur_scope;
        std::unique_ptr<Atom> prev_expr;
        std::list<std::unique_ptr<middleend::mir::BasicBlock>> basic_blocks;
        std::unordered_map<uint64_t, Type *> var_type_mappings;
        std::unordered_map<uint64_t, Type *> function_type_mappings;
    };

} // namespace frontend
