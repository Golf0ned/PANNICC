#include <cstdint>
#include <string>

#include "frontend/utils/symbol_table.h"


namespace frontend {
    std::string toScopedSymbol(std::string symbol, uint64_t scope) {
        return symbol + '_' + std::to_string(scope);
    }


    uint64_t SymbolTable::addSymbol(std::string symbol) {
        if (hasSymbol(symbol)) {
            return symbol_to_id[symbol];
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
        return symbol_to_id.find(symbol) == symbol_to_id.end();
    }
}
