#include <utility>

#include "backend/lir/data_size.h"

namespace backend::lir {
    DataSize fromMir(middleend::mir::Type type) {
        switch (type) {
        case middleend::mir::Type::I32:
            return DataSize::DOUBLEWORD;
        default:
            std::unreachable();
        }
    }

    char toChar(DataSize size) {
        switch (size) {
        case DataSize::BYTE:
            return 'b';
        case DataSize::WORD:
            return 'w';
        case DataSize::DOUBLEWORD:
            return 'l';
        case DataSize::QUADWORD:
            return 'q';
        }
        std::unreachable();
    }
} // namespace backend::lir
