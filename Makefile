BUILD_DIR := build

.PHONY: all build clean distclean

all: build

build:
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

clean:
	cmake --build $(BUILD_DIR) --target clean

distclean:
	rm -rf $(BUILD_DIR)

test: build
	cd $(BUILD_DIR) && ./bin/regression_tests --gtest_brief=1 | grep . | grep -vE "Skipped"
