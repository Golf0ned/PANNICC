file(GLOB_RECURSE headers "*.h" "*.hpp")
file(GLOB_RECURSE sources "*.cpp")

add_library(frontend SHARED ${headers} ${sources})

target_link_libraries(frontend PUBLIC taocpp::pegtl)
target_include_directories(frontend PUBLIC ..)
