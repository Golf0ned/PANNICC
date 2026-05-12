#pragma once

#include "frontend/utils/pegtl_helpers.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {

//
// Symbol Table
//
extern SymbolTable st;

//
// Keywords
//
struct keyword_return : TAO_PEGTL_STRING("return") {};
struct keyword_if : TAO_PEGTL_STRING("if") {};
struct keyword_else : TAO_PEGTL_STRING("else") {};
struct keyword_while : TAO_PEGTL_STRING("while") {};

//
// Types
//
struct keyword_short : TAO_PEGTL_STRING("short") {};
struct keyword_int : TAO_PEGTL_STRING("int") {};
struct keyword_long : TAO_PEGTL_STRING("long") {};
struct keyword_long_long : TAO_PEGTL_STRING("long long") {};

//
// Characters
//
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

//
// Ignorables
//
struct comment
    : pegtl::disable<TAO_PEGTL_STRING("//"), pegtl::until<pegtl::eolf>> {};

struct whitespace_char : pegtl::sor<pegtl::space> {};

struct whitespace : pegtl::plus<whitespace_char> {};

struct ignorable : pegtl::star<pegtl::sor<whitespace_char, comment>> {};

//
// Comma List
//
template <typename Rule>
using pegtl_comma_list = pegtl::list<Rule, pegtl::seq<comma, ignorable>>;

//
// Actions
//
template <typename Rule> struct action {};

} // namespace frontend
