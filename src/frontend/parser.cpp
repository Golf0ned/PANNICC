#include <cstdint>
#include <string>
#include <utility>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/ast/ast.h"
#include "frontend/ast/instruction.h"
#include "frontend/parser.h"
#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

template <typename Rule>
using pegtl_apply_if_match = pegtl::seq<pegtl::at<Rule>, Rule>;

namespace frontend {
    typedef std::pair<std::vector<ast::Function>, SymbolTable> parsing_results;

    std::vector<std::string> parsed_tokens;
    std::vector<Type> parsed_types;
    std::vector<std::unique_ptr<Atom>> parsed_atoms;
    std::vector<std::unique_ptr<ast::Scope>> active_scopes;

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
    struct ampersand : pegtl::one<'&'> {};
    struct equal : pegtl::one<'='> {};

    // Ignorables
    struct comment
        : pegtl::disable<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eolf>> {};

    struct whitespace_char
        : pegtl::sor<pegtl::one<' '>, pegtl::one<'\t'>, pegtl::eol> {};

    struct whitespace : pegtl::plus<whitespace_char> {};

    struct ignorable : pegtl::star<pegtl::sor<whitespace_char, comment>> {};

    // Values
    struct identifier
        : pegtl::seq<pegtl::plus<pegtl::sor<pegtl::alpha, pegtl::one<'_'>>>,
                     pegtl::star<pegtl::sor<pegtl::alnum, pegtl::one<'_'>>>> {};

    struct number
        : pegtl::seq<pegtl::opt<plus, minus>, pegtl::plus<pegtl::digit>> {};

    struct value : pegtl::sor<identifier, number> {};

    struct binary_op : pegtl::sor<plus, minus, asterisk, ampersand> {};

    struct type : pegtl::sor<pegtl_apply_if_match<keyword_long_long>,
                             pegtl_apply_if_match<keyword_long>,
                             pegtl_apply_if_match<keyword_int>,
                             pegtl_apply_if_match<keyword_short>> {};

    // Instructions
    struct instruction_any;
    struct scope;

    struct instruction_declaration
        : pegtl::seq<type, ignorable, identifier, ignorable, semicolon> {};

    struct instruction_declaration_assign_value
        : pegtl::seq<type, ignorable, identifier, ignorable, equal, ignorable,
                     value, ignorable, semicolon> {};

    struct instruction_assign_value
        : pegtl::seq<identifier, ignorable, equal, ignorable, value, ignorable,
                     semicolon> {};

    struct instruction_assign_binary_op
        : pegtl::seq<identifier, ignorable, equal, ignorable, value, ignorable,
                     binary_op, ignorable, value, ignorable, semicolon> {};

    struct instruction_return
        : pegtl::seq<keyword_return, ignorable, value, ignorable, semicolon> {};

    struct instruction_call
        : pegtl::seq<identifier, ignorable, left_paren, ignorable,
                     // TODO: parameter list
                     right_paren, ignorable, semicolon> {};

    struct instruction_call_assign
        : pegtl::seq<identifier, ignorable, equal, ignorable, identifier,
                     ignorable, left_paren, ignorable,
                     // TODO: parameter list
                     right_paren, ignorable, semicolon> {};

    struct instruction_if
        : pegtl::seq<keyword_if, ignorable, left_paren, ignorable, value,
                     ignorable, right_paren, ignorable, instruction_any> {};

    struct instruction_if_else
        : pegtl::seq<keyword_if, ignorable, left_paren, ignorable, value,
                     ignorable, right_paren, ignorable, instruction_any,
                     ignorable, keyword_else, ignorable, instruction_any> {};

    struct instruction_while
        : pegtl::seq<keyword_while, ignorable, left_paren, ignorable, value,
                     ignorable, right_paren, ignorable, instruction_any> {};

    struct instruction_any
        : pegtl::sor<pegtl_apply_if_match<scope>,
                     pegtl_apply_if_match<instruction_while>,
                     pegtl_apply_if_match<instruction_if_else>,
                     pegtl_apply_if_match<instruction_if>,
                     pegtl_apply_if_match<instruction_return>,
                     pegtl_apply_if_match<instruction_call>,
                     pegtl_apply_if_match<instruction_call_assign>,
                     pegtl_apply_if_match<instruction_declaration_assign_value>,
                     pegtl_apply_if_match<instruction_declaration>,
                     pegtl_apply_if_match<instruction_assign_value>,
                     pegtl_apply_if_match<instruction_assign_binary_op>> {};

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
    template <typename Rule> struct action : pegtl::nothing<Rule> {};

    // Value token actions
    template <> struct action<number> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto a = std::make_unique<AtomLiteral>(std::stoull(in.string()),
                                                   parsed_types.back());
            parsed_atoms.push_back(std::move(a));
        }
    };

    template <> struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            Type type;
            uint64_t id;

            if (res.second.hasSymbol(in.string())) {
                id = res.second.addSymbol(in.string(), type);
                type = res.second.getType(id);
            } else {
                type = parsed_types.back();
                id = res.second.addSymbol(in.string(), type);
            }

            auto a = std::make_unique<AtomIdentifier>(id, type);
            parsed_atoms.push_back(std::move(a));
        }
    };

    template <> struct action<binary_op> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            parsed_tokens.push_back(in.string());
        }
    };

    template <> struct action<type> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            parsed_types.push_back(strToType.at(in.string()));
        }
    };

    // Instruction actions
    template <> struct action<left_brace> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto scope = std::make_unique<ast::Scope>();
            active_scopes.push_back(std::move(scope));
        }
    };

    template <> struct action<right_brace> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            if (active_scopes.size() == 1) // function
                return;

            auto scope = std::move(active_scopes.back());
            active_scopes.pop_back();
            active_scopes.back()->addInstruction(std::move(scope));
        }
    };

    template <> struct action<instruction_declaration> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> variable(ptr);

            auto i = std::make_unique<ast::InstructionDeclaration>(
                std::move(variable));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_declaration_assign_value> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto value = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> variable(ptr);

            auto i = std::make_unique<ast::InstructionDeclarationAssignValue>(
                std::move(variable), std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_assign_value> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto value = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> variable(ptr);

            auto i = std::make_unique<ast::InstructionAssignValue>(
                std::move(variable), std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_assign_binary_op> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto right = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            BinaryOp op = strToBinaryOp.at(parsed_tokens.back());
            parsed_tokens.pop_back();

            auto left = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> variable(ptr);

            auto i = std::make_unique<ast::InstructionAssignBinaryOp>(
                std::move(variable), op, std::move(left), std::move(right));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_return> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto value = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto i = std::make_unique<ast::InstructionReturn>(std::move(value));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_call> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> target(ptr);

            auto i = std::make_unique<ast::InstructionCall>(std::move(target));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_call_assign> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> target(ptr);

            back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> variable(ptr);

            auto i = std::make_unique<ast::InstructionCallAssign>(
                std::move(variable), std::move(target));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_if> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto cond = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto t_branch =
                std::move(active_scopes.back()->getInstructions().back());
            active_scopes.back()->getInstructions().pop_back();

            auto i = std::make_unique<ast::InstructionIf>(
                std::move(cond), std::move(t_branch), nullptr);
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_if_else> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto cond = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto f_branch =
                std::move(active_scopes.back()->getInstructions().back());
            active_scopes.back()->getInstructions().pop_back();

            auto t_branch =
                std::move(active_scopes.back()->getInstructions().back());
            active_scopes.back()->getInstructions().pop_back();

            auto i = std::make_unique<ast::InstructionIf>(
                std::move(cond), std::move(t_branch), std::move(f_branch));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    template <> struct action<instruction_while> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto cond = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();

            auto body =
                std::move(active_scopes.back()->getInstructions().back());
            active_scopes.back()->getInstructions().pop_back();

            auto i = std::make_unique<ast::InstructionWhile>(std::move(cond),
                                                             std::move(body));
            active_scopes.back()->addInstruction(std::move(i));
        }
    };

    // Function
    template <> struct action<function> {
        template <typename Input>
        static void apply(const Input &in, parsing_results &res) {
            auto back = std::move(parsed_atoms.back());
            parsed_atoms.pop_back();
            AtomIdentifier *ptr = static_cast<AtomIdentifier *>(back.release());
            std::unique_ptr<AtomIdentifier> name(ptr);

            auto body = std::move(active_scopes.back());
            active_scopes.pop_back();

            res.first.push_back(
                ast::Function(std::move(name), std::move(body)));
        }
    };

    //
    // Parse function
    //
    ast::Program parse(const std::string &input_file) {
        file_input in(input_file);

        parsed_tokens.clear();
        parsed_atoms.clear();
        active_scopes.clear();

        parsing_results res;
        pegtl::parse<grammar, action>(in, res);

        ast::Program ast(std::move(res.first), res.second);
        return ast;
    }
} // namespace frontend
