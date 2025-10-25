#pragma once

#include "middleend/mir/type.h"

namespace backend::lir {
    enum class DataSize {
        BYTE,
        WORD,
        DOUBLEWORD,
        QUADWORD,
    };

    DataSize fromMir(middleend::mir::Type type);

    char toChar(DataSize size);
} // namespace backend::lir
