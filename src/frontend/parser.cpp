#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/parser.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

namespace frontend {

    //
    // Grammar
    //

    // Tokens
    struct keyword_return : TAO_PEGTL_STRING("return") {};

    struct keyword_int64 : TAO_PEGTL_STRING("int64") {};

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
    
    // Ignorables
    struct comment
        : pegtl::disable<TAO_PEGTL_STRING( "//" ),
                         pegtl::until<pegtl::eolf>> {};

    struct whitespace
        : pegtl::star<pegtl::sor<pegtl::one<' '>,
                                 pegtl::one<'\t'>,
                                 pegtl::eol>> {};

    struct ignorable
        : pegtl::star<pegtl::seq<whitespace,
                                 comment>> {};

    // Identifiers and literals
    struct identifier
        : pegtl::seq<pegtl::plus<pegtl::sor<pegtl::alpha,
                                            pegtl::one<'_'>>>,
                     pegtl::star<pegtl::sor<pegtl::alnum,
                                            pegtl::one<'_'>>>> {};

    // Program structure
    struct function
        : pegtl::seq<keyword_int64,
                     ignorable,
                     identifier,
                     ignorable,
                     left_paren,
                     ignorable,
                     // TODO: parameter list
                     right_paren,
                     ignorable,
                     left_brace,
                     // TODO: instruction list
                     right_brace,
                     ignorable,
                     semicolon> {};


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

    AST parse(const std::string &input_file) {
        file_input<> in(input_file);

        AST ast;
        tao::pegtl::parse<grammar, action>(in, ast);
        return ast;
    }
}
