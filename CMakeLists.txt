cmake_minimum_required(VERSION 3.13)
project(PANNICC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    pegtl
    GIT_REPOSITORY https://github.com/taocpp/PEGTL.git
    GIT_TAG 3.2.8
)
FetchContent_MakeAvailable(pegtl)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

install(SCRIPT "cmake/postinstall.cmake")
