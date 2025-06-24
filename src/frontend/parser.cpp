#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include "frontend/parser.h"

namespace pegtl = tao::pegtl;
using namespace pegtl;

namespace frontend {
    struct grammar
        : pegtl::star<pegtl::any> {};

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
