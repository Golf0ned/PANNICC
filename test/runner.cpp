#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"

class CompilerTest : public ::testing::TestWithParam<std::string> {};

static std::string getTestDir() {
    std::filesystem::path file_path = __FILE__;
    return file_path.parent_path().string();
}

TEST_P(CompilerTest, CToMIR) {
    auto param = GetParam();
    std::string input_path = getTestDir() + "/input/" + param + ".c";
    std::string expected_path = getTestDir() + "/expected/" + param + ".mir";

    frontend::ast::Program ast = frontend::parse(input_path);
    frontend::hir::Program hir = frontend::astToHir(ast);
    middleend::mir::Program mir = frontend::hirToMir(hir);
    std::string output = mir.toString() + '\n';

    std::ifstream file(expected_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string expected = buffer.str();

    EXPECT_EQ(expected, output);
}

INSTANTIATE_TEST_SUITE_P(Frontend, CompilerTest, ::testing::Values("scope"));
