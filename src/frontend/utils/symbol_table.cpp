#include <cstdint>
#include <string>

#include "frontend/utils/symbol_table.h"

namespace frontend {
    uint64_t SymbolTable::addSymbol(std::string symbol) {
        if (hasSymbol(symbol)) {
            return symbol_to_id.at(symbol);
        }

        uint64_t id = symbol_to_id.size();
        symbol_to_id[symbol] = id;
        id_to_symbol[id] = symbol;
        return id;
    }

    std::string SymbolTable::getSymbol(uint64_t id) {
        return id_to_symbol.at(id);
    }

    bool SymbolTable::hasSymbol(std::string symbol) {
        return symbol_to_id.contains(symbol);
    }
} // namespace frontend
