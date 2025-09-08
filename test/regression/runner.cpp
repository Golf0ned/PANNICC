#include <algorithm>
#include <filesystem>
#include <fstream>

#include "gtest/gtest.h"

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"

namespace fs = std::filesystem;

void compareIfFileExists(std::string actual, std::string expected_path,
                         std::string ir_name) {
    if (!fs::exists(expected_path))
        GTEST_SKIP() << "Missing comparison output for " << ir_name;

    std::ifstream file(expected_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string expected = buffer.str();
    EXPECT_EQ(expected, actual + '\n');
}

std::string test_dir = fs::path(REGRESSION_TEST_DIR);

class RegressionTest : public ::testing::TestWithParam<std::string> {
protected:
    std::string test_name;
    std::string input_path, ast_path, hir_path, mir_path;

    void SetUp() override {
        test_name = GetParam();
        input_path = test_dir + "/input/" + test_name + ".c";
        ast_path = test_dir + "/expected/" + test_name + ".ast";
        hir_path = test_dir + "/expected/" + test_name + ".hir";
        mir_path = test_dir + "/expected/" + test_name + ".mir";
    }
};

TEST_P(RegressionTest, AST) {
    auto ast = frontend::parse(input_path);
    compareIfFileExists(ast.toString(), ast_path, "ast");
}

TEST_P(RegressionTest, HIR) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    compareIfFileExists(hir.toString(), hir_path, "hir");
}

TEST_P(RegressionTest, MIR) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    compareIfFileExists(mir.toString(), mir_path, "mir");
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
