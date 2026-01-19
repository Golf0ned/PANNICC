#pragma once

#include <cstdint>
#include <string>

namespace backend::lir_tree {
    class FunctionInfo {
    public:
        FunctionInfo(std::string name, uint64_t num_params);
        std::string getName();
        uint64_t getNumParams();
        uint64_t getStackBytes();
        uint64_t allocate(uint64_t num_bytes);

    private:
        std::string name;
        uint64_t num_params;
        uint64_t stack_bytes;
    };
} // namespace backend::lir_tree
