#include <filesystem>
#include <iostream>

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass/mem2reg.h"
#include "middleend/pass/pass.h"
#include "middleend/pass/pass_manager.h"
#include "middleend/pass/simplify_cfg.h"

void printHelp(const std::string &program_name) {
    std::cerr << "USAGE: " << program_name << " [options] <file>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "OPTIONS:" << std::endl;
    std::cerr << "  --help          Show this help message" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  --dump-ast      Print AST only" << std::endl;
    std::cerr << "  --dump-hir      Print HIR only (desugared AST)"
              << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    std::filesystem::path input_file = "";
    bool ast_only = false;
    bool hir_only = false;

    for (int i = 1; i < argc; i++) {
        const std::string &arg = argv[i];

        if (!arg.starts_with("-")) {
            input_file = argv[i];
        } else if (arg == "--help") {
            printHelp(argv[0]);
            return 1;
        } else if (arg == "--dump-ast") {
            ast_only = true;
        } else if (arg == "--dump-hir") {
            hir_only = true;
        } else {
            std::cerr << "Error: invalid option \"" << argv[i] << "\""
                      << std::endl;
            printHelp(argv[0]);
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

    middleend::mir::Program mir = frontend::hirToMir(hir);

    middleend::PassManager pm;
    std::unique_ptr<middleend::TransformPass> mem2reg =
        std::make_unique<middleend::Mem2Reg>();
    pm.addPass(std::move(mem2reg));
    std::unique_ptr<middleend::TransformPass> simplify_cfg =
        std::make_unique<middleend::SimplifyCFG>();
    pm.addPass(std::move(simplify_cfg));
    pm.runPasses(mir);

    std::cout << mir.toString() << std::endl;

    return 0;
}
