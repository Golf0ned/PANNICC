#include <filesystem>
#include <iostream>

#include "frontend/ast/ast.h"
#include "frontend/hir/hir.h"

#include "frontend/ast_to_hir.h"
#include "frontend/parser.h"

void print_help(const std::string &program_name) {
    std::cerr << "USAGE: " << program_name << " [options] <file>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "OPTIONS:" << std::endl;
    std::cerr << "  --help          Show this help message" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  --dump-ast      Print AST only" << std::endl;
    // std::cerr << "  --dump-hir      Print HIR only (desugared AST)" <<
    // std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    std::filesystem::path input_file = "";
    bool ast_only = false;
    bool hir_only = true;

    for (int i = 1; i < argc; i++) {
        const std::string &arg = argv[i];

        if (!arg.starts_with("-")) {
            input_file = argv[i];
        } else if (arg == "--help") {
            print_help(argv[0]);
            return 1;
        } else if (arg == "--dump-ast") {
            ast_only = true;
        }
        // else if (arg == "--dump-hir") {
        //     hir_only = true;
        // }
        else {
            std::cerr << "Error: invalid option \"" << argv[i] << "\""
                      << std::endl;
            print_help(argv[0]);
            return 1;
        }
    }

    if (!std::filesystem::exists(input_file)) {
        std::cerr << "Error: file " << input_file << " does not exist"
                  << std::endl;
        return 1;
    }

    frontend::ast::Program ast = frontend::parse(input_file);
    if (ast_only) {
        std::cout << ast.toString() << std::endl;
        return 0;
    }

    frontend::hir::Program hir = frontend::astToHir(ast);
    if (hir_only) {
        std::cout << hir.toString() << std::endl;
        return 0;
    }

    return 0;
}
