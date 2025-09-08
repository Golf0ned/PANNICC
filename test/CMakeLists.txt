include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests runner.cpp)
target_link_libraries(tests PRIVATE gtest_main frontend middleend)

include(GoogleTest)
gtest_discover_tests(tests)
