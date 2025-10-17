#include <filesystem>
#include <iostream>

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"

#define PRINT(str) (std::cerr << str << std::endl)
#define ERROR(str) (std::cerr << "ERROR: " << str << std::endl)
#define OUTPUT(str) (std::cout << str << std::endl)

enum class OutputLevel { AST, HIR, MIR };

void printHelp(const std::string &program_name) {
    PRINT("USAGE: " + program_name + " [options] <input-file>");
    PRINT("");
    PRINT("OPTIONS:");
    PRINT("  --help               Show this help message");
    PRINT("  -o <file>            Output file");
    PRINT("");
    PRINT("  --dump-ast           Print AST only");
    PRINT("  --dump-hir           Print HIR only (desugared AST)");
    PRINT("");
    PRINT("  -O0|1                Opt level");
    PRINT("  --passes=<passes>    Run comma-separated list of passes");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    std::filesystem::path input_file, output_file;
    OutputLevel output_level = OutputLevel::MIR;
    auto pm = middleend::initializeO1();

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
                ERROR("invalid opt level \"" + arg + "\"");
                return 1;
            }
        } else if (arg.starts_with("--passes=")) {
            std::stringstream stream(arg.substr(9));
            std::string pass;
            while (std::getline(stream, pass, ',')) {
                pm->addPass(pass);
            }
        } else {
            ERROR("invalid option \"" + arg + "\"");
            printHelp(argv[0]);
            return 1;
        }
    }

    if (!std::filesystem::exists(input_file)) {
        ERROR("file \"" + input_file.string() + "\" does not exist");
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
        OUTPUT(ast.toString());
        return 0;
    }

    frontend::hir::Program hir = frontend::astToHir(ast);
    if (output_level == OutputLevel::HIR) {
        OUTPUT(hir.toString());
        return 0;
    }

    middleend::mir::Program mir = frontend::hirToMir(hir);

    pm->runPasses(mir);

    OUTPUT(mir.toString());

    return 0;
}

#undef PRINT
#undef ERROR
#undef OUTPUT
