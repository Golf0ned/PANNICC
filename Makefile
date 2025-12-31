BUILD_DIR := build
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug

.PHONY: all build debug test clean distclean

all: test

build:
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_FLAGS) -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

test: build
	cd $(BUILD_DIR) && ./bin/regression_tests | grep -e "\["

clean:
	if [ -d "$(BUILD_DIR)" ]; then cmake --build $(BUILD_DIR) --target clean; fi

distclean:
	rm -rf $(BUILD_DIR)
