add_executable(pannicc compiler.cpp)
target_link_libraries(pannicc PUBLIC
    frontend
    middleend
    backend
)
if(CMAKE_INSTALL_DIR)
    target_compile_definitions(pannicc PRIVATE
        LIB_DIR="${CMAKE_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}"
    )
else()
    target_compile_definitions(pannicc PRIVATE
        LIB_DIR="${CMAKE_SOURCE_DIR}/install/${CMAKE_INSTALL_LIBDIR}"
    )
endif()

add_library(pannicc_runtime STATIC runtime.c)
target_include_directories(pannicc_runtime PUBLIC ../../include)
