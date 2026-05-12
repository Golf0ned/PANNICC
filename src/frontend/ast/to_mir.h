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

        middleend::mir::InstructionAlloca *
        makeAlloca(middleend::mir::Type type);
        middleend::mir::Value *usePrevExpr();
        middleend::mir::Literal *getLiteral(uint64_t value,
                                            middleend::mir::Type type);
        middleend::mir::Value *getUse(uint64_t id);
        Type *getType(uint64_t id);
        std::vector<std::unique_ptr<middleend::mir::Value>>
        makeParams(std::vector<ast::Parameter> &params);
        void makeBB(std::unique_ptr<middleend::mir::Terminator> terminator);
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
        middleend::mir::InstructionAlloca *ret_alloca;
        std::list<std::unique_ptr<middleend::mir::Instruction>> allocas;
        std::list<std::unique_ptr<middleend::mir::Instruction>> instructions;
        std::list<std::unique_ptr<middleend::mir::BasicBlock>> basic_blocks;

        uint64_t cur_scope;
        std::vector<std::unordered_map<uint64_t, middleend::mir::Value *>>
            scope_bindings;
        std::vector<std::unordered_map<uint64_t, Type *>> scope_binding_types;

        middleend::mir::Value *prev_expr;
        std::unordered_map<ast::Expr *, std::unique_ptr<Type>> expr_types;

        uint64_t cur_bb_id;
        std::unordered_map<uint64_t, middleend::mir::BasicBlock *> bb_ids;
        std::unordered_map<middleend::mir::Terminator *, std::vector<uint64_t>>
            bb_edges;

        std::unordered_map<uint64_t, middleend::mir::Function *> function_ids;
        std::unordered_map<uint64_t, Type *> function_types;
        std::unordered_map<uint64_t,
                           std::vector<middleend::mir::InstructionCall *>>
            function_calls;
    };

} // namespace frontend
