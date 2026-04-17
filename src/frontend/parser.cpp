#include <tao/pegtl/contrib/analyze.hpp>

#include "frontend/parser.h"
#include "frontend/parser/function.h"

namespace frontend {
    struct grammar
        : pegtl::must<pegtl::seq<ignorable, pegtl::star<function>, ignorable>> {
    };

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
