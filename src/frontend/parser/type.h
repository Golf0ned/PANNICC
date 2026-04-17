#pragma once

#include "frontend/ast/ast.h"
#include "frontend/parser/core.h"
#include "frontend/utils/type.h"

namespace frontend {
    extern std::vector<std::unique_ptr<Type>> parsed_types;
    std::unique_ptr<Type> popType();

    //
    // Types
    //
    struct type_int : keyword_int {};
    struct type_ptr : pegtl::seq<ignorable, asterisk> {};
    struct type : pegtl::seq<type_int, pegtl::star<type_ptr>> {};

    //
    // Actions
    //
    template <> struct action<type_int> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto type = std::make_unique<Int>();
            parsed_types.push_back(std::move(type));
        }
    };

    template <> struct action<type_ptr> {
        template <typename Input>
        static void apply(const Input &in, std::vector<ast::Function> &res) {
            auto base = popType();
            auto type = std::make_unique<Ptr>(std::move(base));
            parsed_types.push_back(std::move(type));
        }
    };
} // namespace frontend
