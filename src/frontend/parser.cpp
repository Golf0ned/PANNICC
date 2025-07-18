#include <cstdint>
#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/parser.h"
#include "frontend/ast/atom.h"
#include "frontend/ast/instruction.h"
#include "frontend/type.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

template<typename Rule>
using pegtl_apply_if_match = pegtl::seq<pegtl::at<Rule>, Rule>;

namespace frontend {
    std::vector<std::string> parsed_tokens;
    std::vector<ast::Atom*> parsed_atoms;
    std::unordered_map<std::string, uint64_t> mapped_symbols;
    std::vector<ast::Scope*> active_scopes;

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
        : pegtl::disable<TAO_PEGTL_STRING( "//" ),
                         pegtl::until<pegtl::eolf>> {};

    struct whitespace_char
        : pegtl::sor<pegtl::one<' '>,
                     pegtl::one<'\t'>,
                     pegtl::eol> {};

    struct whitespace
        : pegtl::plus<whitespace_char> {};

    struct ignorable
        : pegtl::star<pegtl::sor<whitespace_char,
                                 comment>> {};

    // Values
    struct identifier
        : pegtl::seq<pegtl::plus<pegtl::sor<pegtl::alpha,
                                            pegtl::one<'_'>>>,
                     pegtl::star<pegtl::sor<pegtl::alnum,
                                            pegtl::one<'_'>>>> {};

    struct number
        : pegtl::seq<pegtl::opt<plus, minus>,
                     pegtl::plus<pegtl::digit>> {};

    struct value
        : pegtl::sor<identifier,
                     number> {};

    struct binary_op
        : pegtl::sor<plus,
                     minus,
                     asterisk,
                     ampersand> {};

    struct type
        : pegtl::sor<keyword_int64_t> {};

    // Instructions
    struct instruction_declaration
        : pegtl::seq<type,
                     ignorable,
                     identifier,
                     ignorable,
                     semicolon> {};

    struct instruction_assign_value
        : pegtl::seq<identifier,
                     ignorable,
                     equal,
                     ignorable,
                     value,
                     ignorable,
                     semicolon> {};

    struct instruction_assign_binary_op
        : pegtl::seq<identifier,
                     ignorable,
                     equal,
                     ignorable,
                     value,
                     ignorable,
                     binary_op,
                     ignorable,
                     value,
                     ignorable,
                     semicolon> {};

    struct instruction_return
        : pegtl::seq<keyword_return,
                     ignorable,
                     value,
                     ignorable,
                     semicolon> {};

    struct scope; // (Forward declaring scope)

    struct instruction_any
        : pegtl::sor<pegtl_apply_if_match<scope>,
                     pegtl_apply_if_match<instruction_declaration>,
                     pegtl_apply_if_match<instruction_assign_value>,
                     pegtl_apply_if_match<instruction_assign_binary_op>,
                     pegtl_apply_if_match<instruction_return>> {};


    // Program structure
    struct scope
        : pegtl::seq<left_brace,
                     ignorable,
                     pegtl::star<pegtl::seq<instruction_any,
                                            ignorable>>,
                     right_brace> {};

    struct function
        : pegtl::seq<type,
                     ignorable,
                     identifier,
                     ignorable,
                     left_paren,
                     ignorable,
                     // TODO: parameter list
                     right_paren,
                     ignorable,
                     scope,
                     ignorable> {};


    struct entry_point
        : pegtl::seq<ignorable,
                     pegtl::star<function>,
                     ignorable> {};

    struct grammar
        : pegtl::must<entry_point> {};


    //
    // Action
    //
    template<typename Rule>
    struct action
        : pegtl::nothing<Rule> {};

    // Value token actions
    template<>
    struct action<number> {
        template <typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::AtomLiteral *a = new ast::AtomLiteral(std::stoull(in.string()));
            parsed_atoms.push_back(a);
        }
    };

    template<>
    struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            uint64_t id;
            std::string identifier = in.string();
         
            if (mapped_symbols.find(identifier) == mapped_symbols.end()) {
                id = mapped_symbols.size();
                ast.addSymbol(id, identifier);
                mapped_symbols[identifier] = id;
            } else {
                id = mapped_symbols[identifier];
            }

            ast::AtomIdentifier *a = new ast::AtomIdentifier(id);
            parsed_atoms.push_back(a);
        }
    };

    template<>
    struct action<binary_op> {
        template <typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            parsed_tokens.push_back(in.string());
        }
    };

    template<>
    struct action<type> {
        template <typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            parsed_tokens.push_back(in.string());
        }
    };

    // Instruction actions
    template<>
    struct action<left_brace> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::Scope* scope = new ast::Scope();
            active_scopes.push_back(scope);
        }
    };

    template<>
    struct action<right_brace> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::Scope* scope = active_scopes.back();
            active_scopes.pop_back();
            if (active_scopes.empty()) {
                // Hold scope for function parse
                active_scopes.push_back(scope);
            } else {
                active_scopes.back()->addInstruction(scope);
            }
        }
    };

    template<>
    struct action<instruction_declaration> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::AtomIdentifier* variable = dynamic_cast<ast::AtomIdentifier*>(parsed_atoms.back());
            parsed_atoms.pop_back();

            Type type = strToType.at(parsed_tokens.back());
            parsed_tokens.pop_back();

            ast::InstructionDeclaration* i = new ast::InstructionDeclaration(type, variable);
            active_scopes.back()->addInstruction(i);
        }
    };

    template<>
    struct action<instruction_assign_value> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::Atom* value = parsed_atoms.back();
            parsed_atoms.pop_back();

            ast::AtomIdentifier* variable = dynamic_cast<ast::AtomIdentifier*>(parsed_atoms.back());
            parsed_atoms.pop_back();

            ast::InstructionAssignValue* i = new ast::InstructionAssignValue(variable, value);
            active_scopes.back()->addInstruction(i);
        }
    };

    template<>
    struct action<instruction_assign_binary_op> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::Atom* right = parsed_atoms.back();
            parsed_atoms.pop_back();

            ast::BinaryOp op = ast::strToBinaryOp.at(parsed_tokens.back());
            parsed_tokens.pop_back();

            ast::Atom* left = parsed_atoms.back();
            parsed_atoms.pop_back();

            ast::AtomIdentifier* variable = dynamic_cast<ast::AtomIdentifier*>(parsed_atoms.back());
            parsed_atoms.pop_back();

            ast::InstructionAssignBinaryOp* i = new ast::InstructionAssignBinaryOp(variable, op, left, right);
            active_scopes.back()->addInstruction(i);
        }
    };

    template<>
    struct action<instruction_return> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::Atom* value = parsed_atoms.back();
            parsed_atoms.pop_back();
            
            ast::InstructionReturn* i = new ast::InstructionReturn(value);
            active_scopes.back()->addInstruction(i);
        }
    };

    // Function
    template<>
    struct action<function> {
        template<typename Input>
        static void apply(const Input &in, ast::Program &ast) {
            ast::AtomIdentifier* name = dynamic_cast<ast::AtomIdentifier*>(parsed_atoms.back());
            parsed_atoms.pop_back();

            Type type = strToType.at(parsed_tokens.back());
            parsed_tokens.pop_back();

            ast::Scope* body = active_scopes.back();
            active_scopes.pop_back();

            ast.addFunction(ast::Function(type, name, body));
        }
    };

    //
    // Parse function
    //
    ast::Program parse(const std::string &input_file) {
        file_input in(input_file);

        parsed_tokens.clear();
        parsed_atoms.clear();
        mapped_symbols.clear();
        active_scopes.clear();

        ast::Program ast;
        pegtl::parse<grammar, action>(in, ast);
        return ast;
    }
}
