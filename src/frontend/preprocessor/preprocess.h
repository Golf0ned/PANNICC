#pragma once

#include "frontend/utils/pegtl_helpers.h"

namespace frontend {
    extern std::vector<std::string> parsed_clauses;

    struct whitespace_char : pegtl::sor<pegtl::one<' '>, pegtl::one<'\t'>> {};
    struct whitespace : pegtl::plus<whitespace_char> {};

    struct keyword_define : TAO_PEGTL_STRING("#define") {};
    struct keyword_include : TAO_PEGTL_STRING("#include") {};

    struct identifier
        : pegtl::seq<pegtl::plus<pegtl::sor<pegtl::alpha, pegtl::one<'_'>>>,
                     pegtl::star<pegtl::sor<pegtl::alnum, pegtl::one<'_'>>>> {};
    struct rest_of_line : pegtl::until<pegtl::eolf> {};

    // TODO: add support for args
    struct directive_define : pegtl::seq<keyword_define, whitespace, identifier,
                                         whitespace, rest_of_line> {};

    struct braces_include
        : pegtl::seq<pegtl::one<'<'>, identifier, pegtl::one<'>'>> {};
    struct quotes_include
        : pegtl::seq<pegtl::one<'\"'>, identifier, pegtl::one<'\"'>> {};
    struct directive_include
        : pegtl::seq<keyword_include, pegtl::opt<whitespace>,
                     pegtl::sor<braces_include, quotes_include>> {};

    struct directive : pegtl::sor<pegtl_try<directive_define>,
                                  pegtl_try<directive_include>> {};

    template <typename Rule> struct action {};

    template <> struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in /*, TYPE res*/) {
            parsed_clauses.push_back(in.string());
        }
    };

    template <> struct action<rest_of_line> {
        template <typename Input>
        static void apply(const Input &in /*, TYPE res*/) {
            parsed_clauses.push_back(in.string());
        }
    };

    template <> struct action<directive_define> {
        template <typename Input>
        static void apply(const Input &in /*, TYPE res*/) {}
    };

    template <> struct action<braces_include> {
        template <typename Input>
        static void apply(const Input &in /*, TYPE res*/) {}
    };

    template <> struct action<quotes_include> {
        template <typename Input>
        static void apply(const Input &in /*, TYPE res*/) {}
    };
} // namespace frontend
