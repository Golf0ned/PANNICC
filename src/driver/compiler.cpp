#include <filesystem>
#include <iostream>
#include <print>

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"

enum class OutputLevel { AST, HIR, MIR };

void printHelp(const std::string &program_name) {
    std::println(std::cerr, "USAGE: {} [options] <file>", program_name);
    std::println(std::cerr);
    std::println(std::cerr, "OPTIONS:");
    std::println(std::cerr, "  --help          Show this help message");
    std::println(std::cerr, "  -o <file>           Opt level");
    std::println(std::cerr);
    std::println(std::cerr, "  --dump-ast      Print AST only");
    std::println(std::cerr, "  --dump-hir      Print HIR only (desugared AST)");
    std::println(std::cerr);
    std::println(std::cerr, "  -O0|1           Opt level");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    std::filesystem::path input_file = "";
    std::filesystem::path output_file;
    OutputLevel output_level = OutputLevel::MIR;
    middleend::PassManager pm = middleend::initializeO1();

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];

        if (!arg.starts_with("-")) {
            input_file = argv[i];
        } else if (arg == "--help") {
            printHelp(argv[0]);
            return 1;
        } else if (arg == "-o") {
            output_file = argv[++i];
        } else if (arg == "--dump-ast") {
            output_level = OutputLevel::AST;
        } else if (arg == "--dump-hir") {
            output_level = OutputLevel::HIR;
        } else if (arg.starts_with("-O") && arg.size() == 3) {
            switch (arg.back()) {
            case '0':
                pm = middleend::initializeO0();
                break;
            case '1':
                pm = middleend::initializeO1();
                break;
            default:
                std::println(std::cerr, "Error: invalid opt level \"-O{}\"",
                             arg.back());
                return 1;
            }
        } else {
            std::println(std::cerr, "Error: invalid option \"{}\"", arg);
            printHelp(argv[0]);
            return 1;
        }
    }

    if (!std::filesystem::exists(input_file)) {
        std::println(std::cerr, "Error: file \"{}\" does not exist",
                     input_file.c_str());
        return 1;
    }

    if (output_file.empty()) {
        output_file = input_file.stem();
        switch (output_level) {
        case OutputLevel::AST:
            output_file.replace_extension(".ast");
            break;
        case OutputLevel::HIR:
            output_file.replace_extension(".hir");
            break;
        case OutputLevel::MIR:
            output_file.replace_extension(".MIR");
            break;
        }
    }

    // TODO: check paths + write to paths

    frontend::ast::Program ast = frontend::parse(input_file);
    if (output_level == OutputLevel::AST) {
        std::println("{}", ast.toString());
        return 0;
    }

    frontend::hir::Program hir = frontend::astToHir(ast);
    if (output_level == OutputLevel::HIR) {
        std::println("{}", hir.toString());
        return 0;
    }

    middleend::mir::Program mir = frontend::hirToMir(hir);

    pm.runPasses(mir);

    std::println("{}", mir.toString());

    return 0;
}
