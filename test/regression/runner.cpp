#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "backend/mir_to_lir.h"
#include "backend/regalloc.h"
#include "frontend/ast_to_hir.h"
#include "frontend/hir_to_mir.h"
#include "frontend/parser.h"
#include "middleend/pass_manager.h"

namespace fs = std::filesystem;

using namespace frontend;
using namespace middleend;
using namespace backend;

std::unique_ptr<PassManager> buildPassManager(std::string passes_path) {
    if (!fs::exists(passes_path))
        return nullptr;

    std::ifstream file(passes_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    auto pm = std::make_unique<PassManager>();

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

void testAst(std::string input_path, std::string expected_path) {
    auto ast = parse(input_path);
    compareIfFileExists(ast.toString(), expected_path);
}

void testHir(std::string input_path, std::string expected_path) {
    auto ast = parse(input_path);
    auto hir = astToHir(ast);
    compareIfFileExists(hir.toString(), expected_path);
}

void testMir(std::string input_path, std::string expected_path,
             PassManager *pm) {
    auto ast = parse(input_path);
    auto hir = astToHir(ast);
    auto mir = hirToMir(hir);
    if (!pm)
        GTEST_SKIP();
    pm->runPasses(mir);
    compareIfFileExists(mir.toString(), expected_path);
}

void testLirInitial(std::string input_path, std::string expected_path,
                    PassManager *pm) {
    auto ast = parse(input_path);
    auto hir = astToHir(ast);
    auto mir = hirToMir(hir);
    if (!pm)
        GTEST_SKIP();
    pm->runPasses(mir);
    auto lir = mirToLir(mir);
    compareIfFileExists(lir.toString(), expected_path);
}

void testLir(std::string input_path, std::string expected_path,
             PassManager *pm) {
    auto ast = parse(input_path);
    auto hir = astToHir(ast);
    auto mir = hirToMir(hir);
    if (!pm)
        GTEST_SKIP();
    pm->runPasses(mir);
    auto lir = mirToLir(mir);
    allocateRegisters(lir);
    compareIfFileExists(lir.toString(), expected_path);
}

std::string test_dir = fs::path(REGRESSION_TEST_DIR);

class RegressionTest : public ::testing::TestWithParam<std::string> {
protected:
    std::string test_name;
    std::string input_path, passes_path;

    void SetUp() override {
        test_name = GetParam();

        input_path = test_dir + "/input/" + test_name + ".c";
        passes_path = test_dir + "/input/" + test_name + ".passes";
    }
};

TEST_P(RegressionTest, AST) {
    auto expected_path = test_dir + "/expected/" + test_name + ".ast";
    testAst(input_path, expected_path);
}

TEST_P(RegressionTest, HIR) {
    auto expected_path = test_dir + "/expected/" + test_name + ".hir";
    testHir(input_path, expected_path);
}

TEST_P(RegressionTest, MIR) {
    auto expected_path = test_dir + "/expected/" + test_name + "_o0.mir";
    testMir(input_path, expected_path, initializeO0().get());
}

TEST_P(RegressionTest, MIRWithO1) {
    auto expected_path = test_dir + "/expected/" + test_name + "_o1.mir";
    testMir(input_path, expected_path, initializeO1().get());
}

TEST_P(RegressionTest, MIRWithSelectPasses) {
    auto expected_path = test_dir + "/expected/" + test_name + "_select.mir";
    testMir(input_path, expected_path, buildPassManager(passes_path).get());
}

TEST_P(RegressionTest, LIRInitial) {
    auto expected_path =
        test_dir + "/expected/" + test_name + "_o0_initial.lir";
    testLirInitial(input_path, expected_path, initializeO0().get());
}

TEST_P(RegressionTest, LIRInitialWithO1) {
    auto expected_path =
        test_dir + "/expected/" + test_name + "_o1_initial.lir";
    testLirInitial(input_path, expected_path, initializeO1().get());
}

TEST_P(RegressionTest, LIRInitialWithSelectPasses) {
    auto expected_path =
        test_dir + "/expected/" + test_name + "_select_initial.lir";
    testLirInitial(input_path, expected_path,
                   buildPassManager(passes_path).get());
}

TEST_P(RegressionTest, LIR) {
    auto expected_path = test_dir + "/expected/" + test_name + "_o0.lir";
    testLir(input_path, expected_path, initializeO0().get());
}

TEST_P(RegressionTest, LIRWithO1) {
    auto expected_path = test_dir + "/expected/" + test_name + "_o1.lir";
    testLir(input_path, expected_path, initializeO1().get());
}

TEST_P(RegressionTest, LIRWithSelectPasses) {
    auto expected_path = test_dir + "/expected/" + test_name + "_select.lir";
    testLir(input_path, expected_path, buildPassManager(passes_path).get());
}

std::vector<std::string> discoverTests(std::string input_dir) {
    std::vector<std::string> res;
    for (auto &f : fs::directory_iterator(input_dir)) {
        auto path = f.path();
        if (f.is_regular_file() && path.extension() == ".c")
            res.push_back(path.stem().string());
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
