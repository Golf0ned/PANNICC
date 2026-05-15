file(GLOB_RECURSE headers "*.h" "*.hpp")
file(GLOB_RECURSE sources "*.cpp")

add_library(backend SHARED ${headers} ${sources})

target_include_directories(backend PUBLIC ..)
