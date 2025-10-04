#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"
#include "middleend/transform/mem2reg.h"
#include "middleend/transform/simplify_cfg.h"

namespace fs = std::filesystem;

std::unique_ptr<middleend::PassManager>
buildPassesFromFile(std::string passes_path) {
    if (!fs::exists(passes_path))
        return nullptr;

    std::ifstream file(passes_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    auto pm = std::make_unique<middleend::PassManager>();

    std::string pass_str;
    while (buffer >> pass_str) {
        if (pass_str == "mem2reg")
            pm->addPass(std::make_unique<middleend::Mem2Reg>());
        else if (pass_str == "simplify_cfg")
            pm->addPass(std::make_unique<middleend::SimplifyCFG>());
        else
            continue;
    }

    return std::move(pm);
}

void compareIfFileExists(std::string actual, std::string expected_path) {
    if (!fs::exists(expected_path))
        GTEST_SKIP();

    std::ifstream file(expected_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string expected = buffer.str();
    EXPECT_EQ(expected, actual + '\n');
}

std::string test_dir = fs::path(REGRESSION_TEST_DIR);

class RegressionTest : public ::testing::TestWithParam<std::string> {
protected:
    std::string test_name;
    std::string input_path, passes_path;
    std::string ast_path, hir_path;
    std::string mir_path, mir_opt_path;

    void SetUp() override {
        test_name = GetParam();

        input_path = test_dir + "/input/" + test_name + ".c";
        passes_path = test_dir + "/input/" + test_name + ".passes";

        ast_path = test_dir + "/expected/" + test_name + ".ast";
        hir_path = test_dir + "/expected/" + test_name + ".hir";

        mir_path = test_dir + "/expected/" + test_name + ".mir";
        mir_opt_path = test_dir + "/expected/" + test_name + "_opt.mir";
    }
};

TEST_P(RegressionTest, AST) {
    auto ast = frontend::parse(input_path);
    compareIfFileExists(ast.toString(), ast_path);
}

TEST_P(RegressionTest, HIR) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    compareIfFileExists(hir.toString(), hir_path);
}

TEST_P(RegressionTest, MIR) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    compareIfFileExists(mir.toString(), mir_path);
}

TEST_P(RegressionTest, MIRWithPasses) {
    auto pm = buildPassesFromFile(passes_path);
    if (!pm)
        GTEST_SKIP();

    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    compareIfFileExists(mir.toString(), mir_opt_path);
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
