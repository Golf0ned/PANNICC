#pragma once

#include "frontend/ast/instruction.h"
#include "frontend/parser/expr.h"
#include "frontend/parser/type.h"

namespace frontend {
    extern BinaryOp last_op_equals;
    extern std::vector<std::unique_ptr<ast::Scope>> active_scopes;
    std::unique_ptr<ast::Instruction> popInstruction();
    std::unique_ptr<ast::Scope> popScope();

    //
    // Op equals
    //
    struct add_equals : pegtl::seq<plus, equal> {};
    struct sub_equals : pegtl::seq<minus, equal> {};
    struct mul_equals : pegtl::seq<asterisk, equal> {};
    struct div_equals : pegtl::seq<slash, equal> {};
    struct lshift_equals : pegtl::seq<less, less, equal> {};
    struct rshift_equals : pegtl::seq<greater, greater, equal> {};
    struct and_equals : pegtl::seq<ampersand, equal> {};
    struct or_equals : pegtl::seq<pipe, equal> {};
    struct xor_equals : pegtl::seq<caret, equal> {};
    struct op_equals : pegtl::sor<add_equals, sub_equals, mul_equals,
                                  div_equals, lshift_equals, rshift_equals,
                                  and_equals, or_equals, xor_equals> {};

    //
    // Instructions
    //
    struct instruction_any;
    struct scope;

    struct instruction_expr : pegtl::seq<expr, ignorable, semicolon> {};

    struct instruction_declaration
        : pegtl::seq<type, ignorable, identifier, ignorable, semicolon> {};

    struct instruction_declaration_assign
        : pegtl::seq<type, ignorable, identifier, ignorable, equal, ignorable,
                     expr, ignorable, semicolon> {};

    struct instruction_assign
        : pegtl::seq<identifier, ignorable, equal, ignorable, expr, ignorable,
                     semicolon> {};

    struct instruction_op_assign
        : pegtl::seq<identifier, ignorable, op_equals, ignorable, expr,
                     ignorable, semicolon> {};

    struct instruction_return
        : pegtl::seq<keyword_return, ignorable, expr, ignorable, semicolon> {};

    struct instruction_if
        : pegtl::seq<keyword_if, ignorable, left_paren, ignorable, expr,
                     ignorable, right_paren, ignorable, instruction_any> {};

    struct instruction_if_else
        : pegtl::seq<keyword_if, ignorable, left_paren, ignorable, expr,
                     ignorable, right_paren, ignorable, instruction_any,
                     ignorable, keyword_else, ignorable, instruction_any> {};

    struct instruction_while
        : pegtl::seq<keyword_while, ignorable, left_paren, ignorable, expr,
                     ignorable, right_paren, ignorable, instruction_any> {};

    struct instruction_any
        : pegtl::sor<pegtl_try<scope>, pegtl_try<instruction_while>,
                     pegtl_try<instruction_if_else>, pegtl_try<instruction_if>,
                     pegtl_try<instruction_return>,
                     pegtl_try<instruction_declaration_assign>,
                     pegtl_try<instruction_declaration>,
                     pegtl_try<instruction_op_assign>,
                     pegtl_try<instruction_assign>,
                     pegtl_try<instruction_expr>> {};

    struct scope
        : pegtl::seq<left_brace, ignorable,
                     pegtl::star<pegtl::seq<instruction_any, ignorable>>,
                     right_brace> {};

    //
    // Actions
    //
    template <> struct action<add_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::ADD;
        }
    };

    template <> struct action<sub_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::SUB;
        }
    };

    template <> struct action<mul_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::MUL;
        }
    };

    template <> struct action<div_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::DIV;
        }
    };

    template <> struct action<lshift_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::LSHIFT;
        }
    };

    template <> struct action<rshift_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::RSHIFT;
        }
    };

    template <> struct action<and_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::AND;
        }
    };

    template <> struct action<or_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::OR;
        }
    };

    template <> struct action<xor_equals> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            last_op_equals = BinaryOp::XOR;
        }
    };

    // Instruction actions
    template <> struct action<left_brace> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto scope = std::make_unique<ast::Scope>();
            active_scopes.push_back(std::move(scope));
        }
    };

    template <> struct action<right_brace> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            if (active_scopes.size() == 1) // function -- in global scope
                return;

            auto scope = std::move(active_scopes.back());
            active_scopes.pop_back();
            active_scopes.back()->addInstruction(std::move(scope));
        }
    };

    template <> struct action<instruction_expr> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto expr = popExpr();

            auto i = std::make_unique<ast::InstructionExpr>(std::move(expr));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_declaration> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto variable = popIdentifier();
            auto type = popType();

            auto i = std::make_unique<ast::InstructionDeclaration>(
                std::move(type), std::move(variable));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_declaration_assign> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popExpr();
            auto variable = popIdentifier();
            auto type = popType();

            auto i = std::make_unique<ast::InstructionDeclarationAssign>(
                std::move(type), std::move(variable), std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_assign> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popExpr();
            auto variable = popIdentifier();

            auto i = std::make_unique<ast::InstructionAssign>(
                std::move(variable), std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_op_assign> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popExpr();
            auto variable = popIdentifier();

            auto i = std::make_unique<ast::InstructionOpAssign>(
                std::move(variable), last_op_equals, std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_return> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popExpr();

            auto i = std::make_unique<ast::InstructionReturn>(std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_if> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto t_branch = popInstruction();
            auto cond = popExpr();

            auto i = std::make_unique<ast::InstructionIf>(
                std::move(cond), std::move(t_branch), nullptr);
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_if_else> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto f_branch = popInstruction();
            auto t_branch = popInstruction();
            auto cond = popExpr();

            auto i = std::make_unique<ast::InstructionIf>(
                std::move(cond), std::move(t_branch), std::move(f_branch));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_while> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto body = popInstruction();
            auto cond = popExpr();

            auto i = std::make_unique<ast::InstructionWhile>(std::move(cond),
                                                             std::move(body));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };
} // namespace frontend
