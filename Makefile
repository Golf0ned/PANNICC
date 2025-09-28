BUILD_DIR := build
DEBUG_DIR := debug

.PHONY: all build debug test clean distclean

all: build debug

build:
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

debug:
	mkdir -p $(DEBUG_DIR)
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B $(DEBUG_DIR)
	cmake --build $(DEBUG_DIR)

test: build
	cd $(BUILD_DIR) && ./bin/regression_tests | grep . | grep -vE "Skipped|SKIPPED"

clean:
	if [ -d "$(BUILD_DIR)" ]; then cmake --build $(BUILD_DIR) --target clean; fi
	if [ -d "$(DEBUG_DIR)" ]; then cmake --build $(DEBUG_DIR) --target clean; fi

distclean:
	rm -rf $(BUILD_DIR) $(DEBUG_DIR)
