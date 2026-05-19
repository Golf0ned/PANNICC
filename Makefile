BUILD_DIR := build
INSTALL_DIR := install
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

all: install test

build:
	mkdir -p $(BUILD_DIR)
	cmake $(CMAKE_FLAGS) -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) -j 32

install: build
	mkdir -p $(INSTALL_DIR)
	cmake --install $(BUILD_DIR) --prefix $(INSTALL_DIR) -j 32

test:
	cd $(BUILD_DIR) && ./test/regression/regression_tests | grep -e "\["

clean:
	cmake --build $(BUILD_DIR) --target clean

distclean:
	rm -rf $(BUILD_DIR) $(INSTALL_DIR) enable

.PHONY: all build debug test clean distclean
