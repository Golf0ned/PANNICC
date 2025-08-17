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
    std::vector<Atom *> parsed_atoms;
    std::vector<ast::Scope *> active_scopes;

    //
    // Grammar
    //
    // Whitespace/ignorables are handled by
    // program structure constructs.
    //

    // Tokens
    struct keyword_return : TAO_PEGTL_STRING("return") {};

    struct keyword_int64_t : TAO_PEGTL_STRING("int64_t") {};

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

    struct type : pegtl::sor<keyword_int64_t> {};

    // Instructions
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

    struct scope; // (Forward declaring scope)

    struct instruction_any
        : pegtl::sor<pegtl_apply_if_match<scope>,
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
                AtomLiteral *a = new AtomLiteral(std::stoull(in.string()));
                parsed_atoms.push_back(a);
            }
    };

    template <> struct action<identifier> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                std::string identifier = in.string();
                uint64_t id = res.second.addSymbol(identifier);

                AtomIdentifier *a = new AtomIdentifier(id);
                parsed_atoms.push_back(a);
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
                parsed_tokens.push_back(in.string());
            }
    };

    // Instruction actions
    template <> struct action<left_brace> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                ast::Scope *scope = new ast::Scope();
                active_scopes.push_back(scope);
            }
    };

    template <> struct action<right_brace> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                ast::Scope *scope = active_scopes.back();
                active_scopes.pop_back();
                if (active_scopes.empty()) {
                    // Hold scope for function parse
                    active_scopes.push_back(scope);
                } else {
                    active_scopes.back()->addInstruction(scope);
                }
            }
    };

    template <> struct action<instruction_declaration> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                AtomIdentifier *variable =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                Type type = strToType.at(parsed_tokens.back());
                parsed_tokens.pop_back();

                ast::InstructionDeclaration *i =
                    new ast::InstructionDeclaration(type, variable);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_declaration_assign_value> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                Atom *value = parsed_atoms.back();
                parsed_atoms.pop_back();

                AtomIdentifier *variable =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                Type type = strToType.at(parsed_tokens.back());
                parsed_tokens.pop_back();

                ast::InstructionDeclarationAssignValue *i =
                    new ast::InstructionDeclarationAssignValue(type, variable,
                                                               value);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_assign_value> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                Atom *value = parsed_atoms.back();
                parsed_atoms.pop_back();

                AtomIdentifier *variable =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                ast::InstructionAssignValue *i =
                    new ast::InstructionAssignValue(variable, value);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_assign_binary_op> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                Atom *right = parsed_atoms.back();
                parsed_atoms.pop_back();

                BinaryOp op = strToBinaryOp.at(parsed_tokens.back());
                parsed_tokens.pop_back();

                Atom *left = parsed_atoms.back();
                parsed_atoms.pop_back();

                AtomIdentifier *variable =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                ast::InstructionAssignBinaryOp *i =
                    new ast::InstructionAssignBinaryOp(variable, op, left,
                                                       right);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_return> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                Atom *value = parsed_atoms.back();
                parsed_atoms.pop_back();

                ast::InstructionReturn *i = new ast::InstructionReturn(value);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_call> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                AtomIdentifier *target =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                ast::InstructionCall *i = new ast::InstructionCall(target);
                active_scopes.back()->addInstruction(i);
            }
    };

    template <> struct action<instruction_call_assign> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                AtomIdentifier *target =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                AtomIdentifier *variable =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                ast::InstructionCallAssign *i =
                    new ast::InstructionCallAssign(variable, target);
                active_scopes.back()->addInstruction(i);
            }
    };

    // Function
    template <> struct action<function> {
            template <typename Input>
            static void apply(const Input &in, parsing_results &res) {
                AtomIdentifier *name =
                    dynamic_cast<AtomIdentifier *>(parsed_atoms.back());
                parsed_atoms.pop_back();

                Type type = strToType.at(parsed_tokens.back());
                parsed_tokens.pop_back();

                ast::Scope *body = active_scopes.back();
                active_scopes.pop_back();

                res.first.push_back(ast::Function(type, name, body));
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

        ast::Program ast(res.first, res.second);
        return ast;
    }
} // namespace frontend
