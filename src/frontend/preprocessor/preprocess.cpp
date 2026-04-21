#include "frontend/preprocessor/preprocess.h"

#include <filesystem>
#include <unordered_map>

namespace frontend {
    std::vector<std::filesystem::path> current_file;
    std::vector<std::string> parsed_clauses;
    std::unordered_map<std::string, std::string> define_map;

    struct whitespace : pegtl::plus<pegtl::blank> {};

    struct keyword_define : TAO_PEGTL_STRING("#define") {};
    struct keyword_include : TAO_PEGTL_STRING("#include") {};

    struct identifier : pegtl::identifier {};
    struct rest_of_line : pegtl::until<pegtl::eolf> {};

    //
    // Body
    //
    struct body_identifier : pegtl::identifier {};
    struct body_other : pegtl::plus<pegtl::not_at<pegtl::identifier_first>,
                                    pegtl::not_at<pegtl::eolf>, pegtl::any> {};
    struct body
        : pegtl::until<pegtl::eolf, pegtl::sor<body_identifier, body_other>> {};

    //
    // Directives
    //
    // TODO: add support for args
    struct directive_define : pegtl::seq<keyword_define, whitespace, identifier,
                                         whitespace, rest_of_line> {};

    struct braces_include
        : pegtl::seq<pegtl::one<'<'>, pegtl::until<pegtl::one<'>'>>> {};
    struct quotes_include
        : pegtl::seq<pegtl::one<'\"'>, pegtl::until<pegtl::one<'\"'>>> {};
    struct directive_include
        : pegtl::seq<keyword_include, pegtl::opt<whitespace>,
                     pegtl::sor<braces_include, quotes_include>,
                     pegtl::until<pegtl::eolf>> {};

    struct directive : pegtl::sor<pegtl_try<directive_define>,
                                  pegtl_try<directive_include>> {};

    //
    // Rest of translation unit
    //
    struct line
        : pegtl::sor<pegtl::seq<pegtl::opt<whitespace>, directive>, body> {};

    struct grammar : pegtl::must<pegtl::until<pegtl::eof, line>> {};

    //
    // Actions
    //
    template <typename Rule> struct action {};

    template <> struct action<body_identifier> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            std::string token = in.string();

            auto replacement = define_map.find(token);
            res +=
                replacement != define_map.end() ? replacement->second : token;
        }
    };

    template <> struct action<body_other> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            res += in.string();
        }
    };

    template <> struct action<body> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            res += '\n';
        }
    };

    template <> struct action<identifier> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            parsed_clauses.push_back(in.string());
        }
    };

    template <> struct action<rest_of_line> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            std::string rest = in.string();
            parsed_clauses.push_back(rest.erase(rest.size() - 1));
        }
    };

    template <> struct action<directive_define> {
        template <typename Input>
        static void apply(const Input &in, std::string res) {
            std::string replacement = parsed_clauses.back();
            parsed_clauses.pop_back();

            std::string to_replace = parsed_clauses.back();
            parsed_clauses.pop_back();

            // TODO: do recursive macro replacement

            define_map[to_replace] = replacement;
        }
    };

    template <> struct action<braces_include> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            parsed_clauses.push_back(in.string());
        }
    };

    template <> struct action<quotes_include> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            parsed_clauses.push_back(in.string());
        }
    };

    template <> struct action<directive_include> {
        template <typename Input>
        static void apply(const Input &in, std::string &res) {
            std::string clause = parsed_clauses.back();
            parsed_clauses.pop_back();

            std::string file_name = clause.substr(1, clause.size() - 2);

            if (clause[0] == '"') {
                auto file = current_file.back().replace_filename(file_name);
                if (std::filesystem::exists(file))
                    res += preprocess(file);

                // TODO: search quote directories
            }

            // TODO: search standard system directories
        }
    };

    std::string preprocess(const std::string &input_file) {
        file_input in(input_file);

        parsed_clauses.clear();

        current_file.push_back((std::filesystem::path)input_file);

        std::string preprocessed;
        pegtl::parse<grammar, action>(in, preprocessed);

        current_file.pop_back();

        return preprocessed;
    }
} // namespace frontend
