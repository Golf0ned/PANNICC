add_subdirectory(frontend)
add_subdirectory(middleend)
add_subdirectory(backend)
add_subdirectory(driver)

install(TARGETS pannicc pannicc_runtime frontend middleend backend)
