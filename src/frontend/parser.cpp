#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/parser.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

template<typename Rule>
using pegtl_apply_if_match = pegtl::seq<pegtl::at<Rule>, Rule>;

namespace frontend {

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

    struct instruction_any
        : pegtl::sor<pegtl_apply_if_match<instruction_declaration>,
                     pegtl_apply_if_match<instruction_assign_value>,
                     pegtl_apply_if_match<instruction_assign_binary_op>,
                     pegtl_apply_if_match<instruction_return>> {};


    // Program structure
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
                     left_brace,
                     ignorable,
                     pegtl::star<pegtl::seq<instruction_any,
                                            ignorable>>,
                     right_brace,
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

    template<>
    struct action<instruction_declaration> {
        template<typename Input>
        static void apply(const Input &in, AST &ast) {
            ast.instructions.emplace_back(new InstructionDeclaration());
        }
    };

    template<>
    struct action<instruction_assign_value> {
        template<typename Input>
        static void apply(const Input &in, AST &ast) {
            ast.instructions.emplace_back(new InstructionAssignValue());
        }
    };

    template<>
    struct action<instruction_assign_binary_op> {
        template<typename Input>
        static void apply(const Input &in, AST &ast) {
            ast.instructions.emplace_back(new InstructionAssignBinaryOp());
        }
    };

    template<>
    struct action<instruction_return> {
        template<typename Input>
        static void apply(const Input &in, AST &ast) {
            ast.instructions.emplace_back(new InstructionReturn());
        }
    };

    //
    // Parse function
    //
    AST parse(const std::string &input_file) {
        file_input in(input_file);

        AST ast;
        pegtl::parse<grammar, action>(in, ast);

        return ast;
    }
}
