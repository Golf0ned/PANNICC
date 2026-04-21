#include <tao/pegtl/contrib/analyze.hpp>

#include "frontend/parser/function.h"
#include "frontend/parser/parser.h"

namespace frontend {
    struct grammar
        : pegtl::must<pegtl::seq<
              ignorable,
              pegtl::opt<pegtl::list<
                  pegtl::sor<pegtl_try<function>, pegtl_try<prototype>>,
                  ignorable>>,
              ignorable>> {};

    ast::Program parse_file(const std::string &input_file) {
        file_input in(input_file);

        symbol_table = std::make_unique<SymbolTable>();
        parsed_tokens.clear();
        active_scopes.clear();

        std::vector<std::unique_ptr<ast::Function>> parsed_functions;
        pegtl::parse<grammar, action>(in, parsed_functions);

        ast::Program ast(std::move(parsed_functions), std::move(symbol_table));
        return ast;
    }

    ast::Program parse_string(const std::string &input) {
        string_input in(input, "string_input");

        symbol_table = std::make_unique<SymbolTable>();
        parsed_tokens.clear();
        active_scopes.clear();

        std::vector<std::unique_ptr<ast::Function>> parsed_functions;
        pegtl::parse<grammar, action>(in, parsed_functions);

        ast::Program ast(std::move(parsed_functions), std::move(symbol_table));
        return ast;
    }
} // namespace frontend
