#include <algorithm>
#include <filesystem>
#include <fstream>

#include "gtest/gtest.h"

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"

namespace fs = std::filesystem;

void compareIfFileExists(std::string expected, std::string actual_path) {
    if (fs::exists(actual_path)) {
        std::ifstream file(actual_path);
        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string actual = buffer.str();
        if (!actual.empty() && actual.back() == '\n')
            actual.pop_back();
        if (expected != actual) {
            FAIL() << "Mismatch with " << actual_path;
        }
    }
}

std::string test_dir = fs::path(REGRESSION_TEST_DIR);

class RegressionTest : public ::testing::TestWithParam<std::string> {};

TEST_P(RegressionTest, FullPipeline) {
    auto test_name = GetParam();
    std::string input_path = test_dir + "/input/" + test_name + ".c";
    std::string ast_path = test_dir + "/expected/" + test_name + ".ast";
    std::string hir_path = test_dir + "/expected/" + test_name + ".hir";
    std::string mir_path = test_dir + "/expected/" + test_name + ".mir";

    frontend::ast::Program ast = frontend::parse(input_path);
    compareIfFileExists(ast.toString(), ast_path);

    frontend::hir::Program hir = frontend::astToHir(ast);
    compareIfFileExists(hir.toString(), hir_path);

    middleend::mir::Program mir = frontend::hirToMir(hir);
    compareIfFileExists(mir.toString(), mir_path);
}

std::vector<std::string> discoverTests(std::string input_dir) {
    std::vector<std::string> res;
    for (auto &f : fs::directory_iterator(input_dir)) {
        auto path = f.path();
        if (f.is_regular_file() && path.extension() == ".c") {
            res.push_back(path.stem().string());
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

INSTANTIATE_TEST_SUITE_P(PANNICC, RegressionTest,
                         ::testing::ValuesIn(discoverTests(test_dir +
                                                           "/input")),
                         [](const ::testing::TestParamInfo<std::string> &info) {
                             return "test_" + info.param;
                         });
