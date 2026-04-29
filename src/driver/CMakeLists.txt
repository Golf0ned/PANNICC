add_executable(pannicc compiler.cpp)
target_link_libraries(pannicc PUBLIC
    frontend
    middleend
    backend
)
target_compile_definitions(pannicc PRIVATE
    LIB_DIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
)
set_target_properties(pannicc PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
)
install(TARGETS pannicc)


add_library(pannicc_runtime STATIC runtime.c)
target_include_directories(pannicc_runtime PUBLIC ../../include)

install(TARGETS pannicc_runtime)
