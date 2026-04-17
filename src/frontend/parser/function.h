#pragma once

#include "frontend/parser/instruction.h"

namespace frontend {
    extern std::vector<ast::Parameter> active_params;

    struct function_param : pegtl::seq<type, ignorable, identifier, ignorable> {
    };
    struct function
        : pegtl::seq<type, ignorable, identifier, ignorable, left_paren,
                     ignorable, pegtl::opt<pegtl_comma_list<function_param>>,
                     right_paren, ignorable, scope, ignorable> {};

    //
    // Actions
    //
    template <> struct action<function_param> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto param_name = popIdentifier();
            auto param_type = popType();
            active_params.push_back(
                {std::move(param_type), std::move(param_name)});
        }
    };

    template <> struct action<function> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto body = popScope();

            auto name = popIdentifier();
            auto type = popType();

            res.push_back(ast::Function(std::move(type), std::move(name),
                                        std::move(active_params),
                                        std::move(body)));
            active_params.clear();
        }
    };

} // namespace frontend
