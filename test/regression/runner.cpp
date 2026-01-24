#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "backend/mir_to_lir.h"
#include "backend/passes/liveness.h"
#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"

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
        pm->addPass(pass_str);
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
    std::string mir_path, mir_o1_path, mir_select_path;
    std::string lir_path, lir_o1_path, lir_select_path;

    void SetUp() override {
        test_name = GetParam();

        input_path = test_dir + "/input/" + test_name + ".c";
        passes_path = test_dir + "/input/" + test_name + ".passes";

        ast_path = test_dir + "/expected/" + test_name + ".ast";
        hir_path = test_dir + "/expected/" + test_name + ".hir";

        mir_path = test_dir + "/expected/" + test_name + "_o0.mir";
        mir_o1_path = test_dir + "/expected/" + test_name + "_o1.mir";
        mir_select_path = test_dir + "/expected/" + test_name + "_select.mir";

        lir_path = test_dir + "/expected/" + test_name + "_o0.lir";
        lir_o1_path = test_dir + "/expected/" + test_name + "_o1.lir";
        lir_select_path = test_dir + "/expected/" + test_name + "_select.lir";
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

TEST_P(RegressionTest, MIRWithO1) {
    auto pm = middleend::initializeO1();
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    compareIfFileExists(mir.toString(), mir_o1_path);
}

TEST_P(RegressionTest, MIRWithSelectPasses) {
    auto pm = buildPassesFromFile(passes_path);
    if (!pm)
        GTEST_SKIP();

    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    compareIfFileExists(mir.toString(), mir_select_path);
}

TEST_P(RegressionTest, LIRInitial) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    auto lir = backend::mirToLir(mir);
    compareIfFileExists(lir.toString(), lir_path);
}

TEST_P(RegressionTest, LIRInitialWithO1) {
    auto pm = middleend::initializeO1();
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    auto lir = backend::mirToLir(mir);
    compareIfFileExists(lir.toString(), lir_o1_path);
}

TEST_P(RegressionTest, LIRInitialWithSelectPasses) {
    auto pm = buildPassesFromFile(passes_path);
    if (!pm)
        GTEST_SKIP();

    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    auto lir = backend::mirToLir(mir);
    compareIfFileExists(lir.toString(), lir_select_path);
}

TEST_P(RegressionTest, LIR) {
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    auto lir = backend::mirToLir(mir);
    compareIfFileExists(lir.toString(), lir_path);
    auto liveness = backend::computeLiveness(lir);
}

TEST_P(RegressionTest, LIRWithO1) {
    auto pm = middleend::initializeO1();
    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    auto lir = backend::mirToLir(mir);
    auto liveness = backend::computeLiveness(lir);
    compareIfFileExists(lir.toString(), lir_o1_path);
}

TEST_P(RegressionTest, LIRWithSelectPasses) {
    auto pm = buildPassesFromFile(passes_path);
    if (!pm)
        GTEST_SKIP();

    auto ast = frontend::parse(input_path);
    auto hir = frontend::astToHir(ast);
    auto mir = frontend::hirToMir(hir);
    pm->runPasses(mir);
    auto lir = backend::mirToLir(mir);
    auto liveness = backend::computeLiveness(lir);
    compareIfFileExists(lir.toString(), lir_select_path);
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
