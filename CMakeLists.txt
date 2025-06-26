cmake_minimum_required(VERSION 3.10)

project(PANNICC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    pegtl
    GIT_REPOSITORY https://github.com/taocpp/PEGTL.git
    GIT_TAG 3.2.8
)
FetchContent_MakeAvailable(pegtl)

add_executable(pannicc
    src/main.cpp
    src/frontend/parser.cpp
    src/frontend/type.cpp
    src/frontend/ast/ast.cpp
    src/frontend/ast/atom.cpp
    src/frontend/ast/instruction.cpp
)

target_include_directories(pannicc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(pannicc PRIVATE taocpp::pegtl)
