file(GLOB_RECURSE sources "*.cpp")

add_executable(unit_tests ${sources})

target_link_libraries(unit_tests PRIVATE
    gtest_main
    frontend
    middleend
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
