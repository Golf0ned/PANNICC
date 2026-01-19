#include "function_info.h"

namespace backend::lir_tree {
    FunctionInfo::FunctionInfo(std::string name, uint64_t num_params)
        : name(name), num_params(num_params), stack_bytes(0) {}

    std::string FunctionInfo::getName() { return name; }

    uint64_t FunctionInfo::getNumParams() { return num_params; }

    uint64_t FunctionInfo::getStackBytes() { return stack_bytes; }

    uint64_t FunctionInfo::allocate(uint64_t num_bytes) {
        auto offset = stack_bytes;
        // TODO: figure out alignment
        stack_bytes += num_bytes;
        return offset;
    }
} // namespace backend::lir_tree
