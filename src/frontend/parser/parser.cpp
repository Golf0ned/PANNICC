#include "frontend/parser/parser.h"
#include "frontend/parser/function.h"

#include <tao/pegtl/contrib/analyze.hpp>

using namespace frontend;

struct grammar
    : pegtl::must<
          pegtl::seq<ignorable,
                     pegtl::opt<pegtl::list<
                         pegtl::sor<pegtl_try<function>, pegtl_try<prototype>>,
                         ignorable>>,
                     ignorable>> {};

ast::Program parse_file(const std::string &input_file) {
    pegtl::file_input in(input_file);

    st = SymbolTable();
    parsed_tokens.clear();
    active_scopes.clear();

    std::vector<std::unique_ptr<ast::Function>> parsed_functions;
    pegtl::parse<grammar, action>(in, parsed_functions);

    ast::Program ast(std::move(parsed_functions), std::move(st));
    return ast;
}

ast::Program parse_string(const std::string &input) {
    pegtl::string_input in(input, "string_input");

    st = SymbolTable();
    parsed_tokens.clear();
    active_scopes.clear();

    std::vector<std::unique_ptr<ast::Function>> parsed_functions;
    pegtl::parse<grammar, action>(in, parsed_functions);

    ast::Program ast(std::move(parsed_functions), std::move(st));
    return ast;
}
