#pragma once

#include "frontend/parser/instruction.h"

namespace frontend {
    extern std::vector<ast::Parameter> active_params;

    struct function_param : pegtl::seq<type, ignorable, identifier, ignorable> {
    };
    struct function_header
        : pegtl::seq<type, ignorable, identifier, ignorable, left_paren,
                     ignorable, pegtl::opt<pegtl_comma_list<function_param>>,
                     right_paren> {};

    struct function : pegtl::seq<function_header, ignorable, scope> {};
    struct prototype : pegtl::seq<function_header, ignorable, semicolon> {};

    //
    // Actions
    //
    template <> struct action<function_param> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto param_name = popIdentifier();
            auto param_type = popType();
            active_params.push_back(
                {std::move(param_type), std::move(param_name)});
        }
    };

    template <> struct action<function> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto body = popScope();

            auto name = popIdentifier();
            auto type = popType();

            auto function = std::make_unique<ast::FunctionDefinition>(
                std::move(type), std::move(name), std::move(active_params),
                std::move(body));

            res.push_back(std::move(function));
            active_params.clear();
        }
    };

    template <> struct action<prototype> {
        template <typename Input>
        static void apply(const Input &in,
                          std::vector<std::unique_ptr<ast::Function>> &res) {
            auto name = popIdentifier();
            auto type = popType();

            auto prototype = std::make_unique<ast::FunctionPrototype>(
                std::move(type), std::move(name), std::move(active_params));

            res.push_back(std::move(prototype));
            active_params.clear();
        }
    };

} // namespace frontend
