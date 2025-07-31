cmake_minimum_required(VERSION 3.11)

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
    src/frontend/ast_to_hir.cpp
    src/frontend/utils/atom.cpp
    src/frontend/utils/operator.cpp
    src/frontend/utils/symbol_table.cpp
    src/frontend/utils/type.cpp
    src/frontend/ast/ast.cpp
    src/frontend/ast/instruction.cpp
    src/frontend/hir/hir.cpp
    src/frontend/hir/instruction.cpp
)

target_include_directories(pannicc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(pannicc PRIVATE taocpp::pegtl)
