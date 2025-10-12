#include <string>
#include <utility>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/ast/ast.h"
#include "frontend/ast/expr.h"
#include "frontend/ast/instruction.h"
#include "frontend/parser.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

template <typename Rule> using pegtl_try = pegtl::seq<pegtl::at<Rule>, Rule>;

namespace frontend {
    enum class TokenType {
        NUMBER,
        IDENTIFIER,
    };

    using Token = std::pair<std::string, TokenType>;

    std::unique_ptr<SymbolTable> symbol_table;
    std::vector<Token> parsed_tokens;
    std::vector<std::unique_ptr<ast::Expr>> parsed_exprs;
    std::vector<std::unique_ptr<ast::Scope>> active_scopes;

    std::unique_ptr<Atom> popAtom() {
        auto [token_val, token_type] = parsed_tokens.back();
        parsed_tokens.pop_back();
        if (token_type == TokenType::NUMBER) {
            long long parsed_val = std::stoll(token_val);
            int32_t truncated = static_cast<int32_t>(parsed_val);
            uint64_t val =
                static_cast<uint64_t>(static_cast<int64_t>(truncated));
            return std::make_unique<AtomLiteral>(val);
        } else
            return std::make_unique<AtomIdentifier>(
                symbol_table->addSymbol(token_val));
    }

    std::unique_ptr<AtomIdentifier> popIdentifier() {
        auto [token_val, token_type] = parsed_tokens.back();
        parsed_tokens.pop_back();
        return std::make_unique<AtomIdentifier>(
            symbol_table->addSymbol(token_val));
    }

    std::unique_ptr<ast::Expr> popExpr() {
        auto expr = std::move(parsed_exprs.back());
        parsed_exprs.pop_back();
        return expr;
    }

    std::unique_ptr<ast::Instruction> popInstruction() {
        auto i = std::move(active_scopes.back()->getInstructions().back());
        active_scopes.back()->getInstructions().pop_back();
        return i;
    }

    std::unique_ptr<ast::Scope> popScope() {
        auto scope = std::move(active_scopes.back());
        active_scopes.pop_back();
        return scope;
    }

    //
    // Grammar
    //
    // Whitespace/ignorables are handled by
    // program structure constructs.
    //

    // Tokens
    struct keyword_return : TAO_PEGTL_STRING("return") {};
    struct keyword_if : TAO_PEGTL_STRING("if") {};
    struct keyword_else : TAO_PEGTL_STRING("else") {};
    struct keyword_while : TAO_PEGTL_STRING("while") {};

    struct keyword_short : TAO_PEGTL_STRING("short") {};
    struct keyword_int : TAO_PEGTL_STRING("int") {};
    struct keyword_long : TAO_PEGTL_STRING("long") {};
    struct keyword_long_long : TAO_PEGTL_STRING("long long") {};

    struct left_paren : pegtl::one<'('> {};
    struct right_paren : pegtl::one<')'> {};
    struct left_brace : pegtl::one<'{'> {};
    struct right_brace : pegtl::one<'}'> {};
    struct comma : pegtl::one<','> {};
    struct semicolon : pegtl::one<';'> {};
    struct plus : pegtl::one<'+'> {};
    struct minus : pegtl::one<'-'> {};
    struct asterisk : pegtl::one<'*'> {};
    struct slash : pegtl::one<'/'> {};
    struct ampersand : pegtl::one<'&'> {};
    struct pipe : pegtl::one<'|'> {};
    struct caret : pegtl::one<'^'> {};
    struct tilde : pegtl::one<'~'> {};
    struct bang : pegtl::one<'!'> {};
    struct equal : pegtl::one<'='> {};
    struct greater : pegtl::one<'>'> {};
    struct less : pegtl::one<'<'> {};

    // Ignorables
    struct comment
        : pegtl::disable<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eolf>> {};

    struct whitespace_char
        : pegtl::sor<pegtl::one<' '>, pegtl::one<'\t'>, pegtl::eol> {};

    struct whitespace : pegtl::plus<whitespace_char> {};

    struct ignorable : pegtl::star<pegtl::sor<whitespace_char, comment>> {};

    // Expression
    // Refer to operator precedence:
    // https://en.cppreference.com/w/c/language/operator_precedence.html

    // 0
    struct identifier
        : pegtl::seq<pegtl::plus<pegtl::sor<pegtl::alpha, pegtl::one<'_'>>>,
                     pegtl::star<pegtl::sor<pegtl::alnum, pegtl::one<'_'>>>> {};

    struct number : pegtl::seq<pegtl::opt<pegtl::sor<plus, minus>>,
                               pegtl::plus<pegtl::digit>> {};
    // 1
    struct expr_1;
    struct call : pegtl::seq<identifier, ignorable, left_paren, ignorable,
                             // TODO: parameter list
                             right_paren> {};
    struct value : pegtl::sor<identifier, number> {};
    struct expr_1 : pegtl::sor<pegtl_try<call>, value> {};

    // 2
    struct expr_2;
    struct unary_plus : pegtl_try<pegtl::seq<plus, ignorable, expr_2>> {};
    struct unary_minus : pegtl_try<pegtl::seq<minus, ignorable, expr_2>> {};
    // struct unary_logical_not : pegtl::seq<bang, ignorable, expr_2> {};
    // struct unary_bitwise_not : pegtl::seq<tilde, ignorable, expr_2> {};
    struct expr_2 : pegtl::sor<expr_1, unary_plus, unary_minus> {};

    // 3
    struct expr_3;
    struct multiply
        : pegtl_try<pegtl::seq<ignorable, asterisk, ignorable, expr_2>> {};
    // struct divide : pegtl::seq<expr_3, ignorable, slash, ignorable, expr_2>
    // {};
    struct expr_3 : pegtl::seq<expr_2, pegtl::star<multiply>> {};

    // 4
    struct expr_4;
    struct add : pegtl_try<pegtl::seq<ignorable, plus, ignorable, expr_3>> {};
    struct subtract
        : pegtl_try<pegtl::seq<ignorable, minus, ignorable, expr_3>> {};
    struct expr_4 : pegtl::seq<expr_3, pegtl::star<pegtl::sor<add, subtract>>> {
    };

    // 8
    struct bitwise_and
        : pegtl_try<pegtl::seq<ignorable, ampersand, ignorable, expr_4>> {};
    struct expr_8 : pegtl::seq<expr_4, pegtl::star<bitwise_and>> {};

    struct expr : expr_8 {};

    struct type
        : pegtl::sor<pegtl_try<keyword_long_long>, pegtl_try<keyword_long>,
                     pegtl_try<keyword_int>, pegtl_try<keyword_short>> {};

    // Instructions
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
                     pegtl_try<instruction_assign>,
                     pegtl_try<instruction_expr>> {};

    // Program structure
    struct scope
        : pegtl::seq<left_brace, ignorable,
                     pegtl::star<pegtl::seq<instruction_any, ignorable>>,
                     right_brace> {};

    struct function : pegtl::seq<type, ignorable, identifier, ignorable,
                                 left_paren, ignorable,
                                 // TODO: parameter list
                                 right_paren, ignorable, scope, ignorable> {};

    struct entry_point
        : pegtl::seq<ignorable, pegtl::star<function>, ignorable> {};

    struct grammar : pegtl::must<entry_point> {};

    //
    // Action
    //
    template <typename Rule> struct action {};

    // Value token actions
    template <> struct action<number> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            parsed_tokens.push_back({in.string(), TokenType::NUMBER});
        }
    };

    template <> struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            parsed_tokens.push_back({in.string(), TokenType::IDENTIFIER});
        }
    };

    // Expr actions
    template <> struct action<value> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popAtom();
            auto expr = std::make_unique<ast::TerminalExpr>(std::move(value));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<call> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto callee = popIdentifier();
            auto expr = std::make_unique<ast::CallExpr>(std::move(callee));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<unary_plus> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::PLUS,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<unary_minus> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto back = popExpr();
            auto expr = std::make_unique<ast::UnaryOpExpr>(UnaryOp::MINUS,
                                                           std::move(back));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<multiply> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::MUL, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<add> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::ADD, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<subtract> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::SUB, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
        }
    };

    template <> struct action<bitwise_and> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto right = popExpr();
            auto left = popExpr();
            auto expr = std::make_unique<ast::BinaryOpExpr>(
                BinaryOp::AND, std::move(left), std::move(right));
            parsed_exprs.push_back(std::move(expr));
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
            auto type = Type::INT;

            auto i = std::make_unique<ast::InstructionDeclaration>(
                type, std::move(variable));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_declaration_assign> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto value = popExpr();
            auto variable = popIdentifier();
            auto type = Type::INT;

            auto i = std::make_unique<ast::InstructionDeclarationAssign>(
                type, std::move(variable), std::move(value));
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

    // Function
    template <> struct action<function> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto body = popScope();
            auto name = popIdentifier();
            auto type = Type::INT;

            res.push_back(
                ast::Function(type, std::move(name), std::move(body)));
        }
    };

    //
    // Parse function
    //
    ast::Program parse(const std::string &input_file) {
        file_input in(input_file);

        symbol_table = std::make_unique<SymbolTable>();
        parsed_tokens.clear();
        active_scopes.clear();

        std::vector<ast::Function> parsed_functions;
        pegtl::parse<grammar, action>(in, parsed_functions);

        ast::Program ast(std::move(parsed_functions), std::move(symbol_table));
        return ast;
    }
} // namespace frontend
