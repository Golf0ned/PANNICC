#include <filesystem>
#include <fstream>
#include <iostream>

#include "backend/codegen.h"
#include "backend/mir_to_lir.h"
#include "backend/regalloc.h"
#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"

#define PRINT(str) (std::cerr << str << std::endl)
#define ERROR(str) (std::cerr << "ERROR: " << str << std::endl)

enum class OutputLevel { AST, HIR, MIR, LIR_ISEL, LIR_REGALLOC, ASM };

void printHelp(const std::string &program_name) {
    PRINT("USAGE: " + program_name + " [options] <input-file>");
    PRINT("");
    PRINT("OPTIONS:");
    PRINT("  --help                 Show this help message");
    PRINT("  -o <file>              Output file");
    PRINT("");
    PRINT("  --emit-ast             Output AST to file");
    PRINT("  --emit-hir             Output HIR to file");
    PRINT("");
    PRINT("  --emit-mir             Output MIR to file");
    PRINT("  -O0|1                  Opt level (defualt O1)");
    PRINT("  --passes=<passes>      Run comma-separated list of passes");
    PRINT("");
    PRINT("  --emit-lir-isel        Output LIR to file, after instruction "
          "selection");
    PRINT("  --emit-lir-regalloc    Output LIR to file, after register "
          "allocation");
}

void output(const std::string &output, std::filesystem::path output_path) {
    std::ofstream output_file(output_path.c_str());
    if (!output_file.is_open())
        ERROR("failed to open file \"" + output_path.string() + "\"");

    output_file << output;

    output_file.close();
    if (!output_file)
        ERROR("writing to file \"" + output_path.string() + "\" failed");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    std::filesystem::path input_file, output_file;
    OutputLevel output_level = OutputLevel::ASM;
    auto pm = middleend::initializeO1();

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];

        if (!arg.starts_with("-")) {
            input_file = argv[i];
        } else if (arg == "--help") {
            printHelp(argv[0]);
            return 1;
        } else if (arg == "-o") {
            if (++i == argc) {
                ERROR("no output file provided");
                printHelp(argv[0]);
                return 1;
            }
            output_file = argv[i];
        } else if (arg == "--emit-ast") {
            output_level = OutputLevel::AST;
        } else if (arg == "--emit-hir") {
            output_level = OutputLevel::HIR;
        } else if (arg == "--emit-mir") {
            output_level = OutputLevel::MIR;
        } else if (arg == "--emit-lir-isel") {
            output_level = OutputLevel::LIR_ISEL;
        } else if (arg == "--emit-lir-regalloc") {
            output_level = OutputLevel::LIR_REGALLOC;
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
            output_file.replace_extension(".mir");
            break;
        case OutputLevel::LIR_ISEL:
        case OutputLevel::LIR_REGALLOC:
            output_file.replace_extension(".lir");
            break;
        case OutputLevel::ASM:
            output_file.replace_extension(".s");
            break;
        }
    }

    frontend::ast::Program ast = frontend::parse(input_file);
    if (output_level == OutputLevel::AST) {
        output(ast.toString(), output_file);
        return 0;
    }

    frontend::hir::Program hir = frontend::astToHir(ast);
    if (output_level == OutputLevel::HIR) {
        output(hir.toString(), output_file);
        return 0;
    }

    middleend::mir::Program mir = frontend::hirToMir(hir);

    pm->runPasses(mir);
    if (output_level == OutputLevel::MIR) {
        output(mir.toString(), output_file);
        return 0;
    }

    backend::lir::Program lir = backend::mirToLir(mir);
    if (output_level == OutputLevel::LIR_ISEL) {
        output(lir.toString(), output_file);
        return 0;
    }

    backend::allocateRegisters(lir);

    if (output_level == OutputLevel::LIR_REGALLOC) {
        output(lir.toString(), output_file);
        return 0;
    }

    std::string assembly = backend::generateCode(lir);
    output(assembly, output_file);

    return 0;
}

#undef PRINT
#undef ERROR
#undef output
