add_executable(pannicc compiler.cpp)
target_link_libraries(pannicc PRIVATE
    frontend
    middleend
    backend
)
target_compile_definitions(pannicc PRIVATE LIB_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_library(pannicc_runtime STATIC runtime.c)
target_include_directories(pannicc_runtime PUBLIC ../../include)
