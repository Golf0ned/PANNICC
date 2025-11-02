add_executable(pannicc compiler.cpp)

target_link_libraries(pannicc PRIVATE
    frontend
    middleend
    backend
)
