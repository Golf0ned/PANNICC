add_executable(regression_tests runner.cpp)

target_link_libraries(regression_tests PRIVATE
    gtest_main
    backend
    frontend
    middleend
)

target_compile_definitions(regression_tests PRIVATE
    REGRESSION_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

include(GoogleTest)
gtest_discover_tests(regression_tests)
