#pragma once

#include "frontend/ast/ast.h"
#include "frontend/ast/expr.h"
#include "frontend/parser/core.h"

namespace frontend {
    enum class TokenType {
        NUMBER,
        IDENTIFIER,
    };
    using Token = std::pair<std::string, TokenType>;

    extern std::vector<std::unique_ptr<ast::Expr>> active_args;
    extern std::vector<Token> parsed_tokens;
    extern std::vector<std::unique_ptr<ast::Expr>> parsed_exprs;
    std::unique_ptr<Atom> popAtom();
    std::unique_ptr<AtomIdentifier> popIdentifier();
    std::unique_ptr<ast::Expr> popExpr();

    //
    // Refer to operator precedence:
    // https://en.cppreference.com/w/c/language/operator_precedence.html
    //
    struct expr;

    //
    // 0
    //
    struct identifier : pegtl::identifier {};

    struct number : pegtl::seq<pegtl::opt<pegtl::sor<plus, minus>>,
                               pegtl::plus<pegtl::digit>> {};

    //
    // 1
    //
    struct expr_1;
    struct call_arg : pegtl::seq<expr, ignorable> {};
    struct call
        : pegtl::seq<identifier, ignorable, left_paren, ignorable,
                     pegtl::opt<pegtl_comma_list<call_arg>>, right_paren> {};
    struct parens
        : pegtl::seq<left_paren, ignorable, expr, ignorable, right_paren> {};
    struct value : pegtl::sor<identifier, number> {};
    struct expr_1 : pegtl::sor<pegtl_try<call>, pegtl_try<parens>, value> {};

    //
    // 2
    //
    struct expr_2;
    struct unary_plus : pegtl_try<pegtl::seq<plus, ignorable, expr_2>> {};
    struct unary_minus : pegtl_try<pegtl::seq<minus, ignorable, expr_2>> {};
    // struct unary_logical_not : pegtl::seq<bang, ignorable, expr_2> {};
    struct unary_bitwise_not : pegtl_try<pegtl::seq<tilde, ignorable, expr_2>> {
    };
    struct deref : pegtl_try<pegtl::seq<asterisk, ignorable, expr_2>> {};
    struct address : pegtl_try<pegtl::seq<ampersand, ignorable, expr_2>> {};
    struct expr_2 : pegtl::sor<expr_1, unary_plus, unary_minus,
                               unary_bitwise_not, deref, address> {};

    //
    // 3
    //
    struct expr_3;
    struct multiply
        : pegtl_try<pegtl::seq<ignorable, asterisk, ignorable, expr_2>> {};
    struct divide : pegtl_try<pegtl::seq<ignorable, slash, ignorable, expr_2>> {
    };
    struct expr_3
        : pegtl::seq<expr_2, pegtl::star<pegtl::sor<multiply, divide>>> {};

    //
    // 4
    //
    struct expr_4;
    struct add : pegtl_try<pegtl::seq<ignorable, plus, ignorable, expr_3>> {};
    struct subtract
        : pegtl_try<pegtl::seq<ignorable, minus, ignorable, expr_3>> {};
    struct expr_4 : pegtl::seq<expr_3, pegtl::star<pegtl::sor<add, subtract>>> {
    };

    //
    // 5
    //
    struct expr_5;
    struct left_shift
        : pegtl_try<pegtl::seq<ignorable, less, less, ignorable, expr_4>> {};
    struct right_shift
        : pegtl_try<
              pegtl::seq<ignorable, greater, greater, ignorable, expr_4>> {};
    struct expr_5
        : pegtl::seq<expr_4, pegtl::star<pegtl::sor<left_shift, right_shift>>> {
    };

    //
    // 8
    //
    struct bitwise_and
        : pegtl_try<pegtl::seq<ignorable, ampersand, ignorable, expr_5>> {};
    struct expr_8 : pegtl::seq<expr_5, pegtl::star<bitwise_and>> {};

    //
    // 9
    //
    struct bitwise_xor
        : pegtl_try<pegtl::seq<ignorable, caret, ignorable, expr_8>> {};
    struct expr_9 : pegtl::seq<expr_8, pegtl::star<bitwise_xor>> {};

    //
    // 10
    //
    struct bitwise_or
        : pegtl_try<pegtl::seq<ignorable, pipe, ignorable, expr_8>> {};
    struct expr_10 : pegtl::seq<expr_9, pegtl::star<bitwise_or>> {};

    struct expr : expr_10 {};

    //
    // Actions
    //
    template <> struct action<number> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            parsed_tokens.push_back({in.string(), TokenType::NUMBER});
        }
    };

    template <> struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            parsed_tokens.push_back({in.string(), TokenType::IDENTIFIER});
        }
    };

    template <> struct action<value> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto value = popAtom();
            auto expr = std::make_unique<ast::TerminalExpr>(std::move(value));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<call_arg> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto arg = popExpr();
            active_args.push_back(std::move(arg));
        }
    };

    template <> struct action<call> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto callee = popIdentifier();
            auto expr = std::make_unique<ast::CallExpr>(std::move(callee),
                                                        std::move(active_args));
            active_args.clear();
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<parens> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::ParenExpr>(std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<unary_plus> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::PLUS,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<unary_minus> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::MINUS,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<unary_bitwise_not> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::NOT,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<deref> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::DEREF,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<address> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::ADDRESS,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<multiply> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::MUL, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<divide> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::DIV, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<add> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::ADD, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<subtract> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::SUB, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<left_shift> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::LSHIFT, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<right_shift> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::RSHIFT, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<bitwise_and> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::AND, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<bitwise_xor> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::XOR, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<bitwise_or> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::OR, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

} // namespace frontend
