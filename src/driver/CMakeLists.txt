add_executable(pannicc compiler.cpp)
target_link_libraries(pannicc PUBLIC
    frontend
    middleend
    backend
)

add_library(pannicc_runtime STATIC runtime.c)
target_include_directories(pannicc_runtime PUBLIC ../../include)
