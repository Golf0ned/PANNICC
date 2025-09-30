#include <cstdint>
#include <stdexcept>
#include <string>

#include "frontend/utils/symbol_table.h"

namespace frontend {
    std::string toScopedSymbol(std::string symbol, uint64_t scope) {
        return symbol + '_' + std::to_string(scope);
    }

    uint64_t SymbolTable::addSymbol(std::string symbol, Type type) {
        if (symbol_to_id.contains(symbol)) {
            uint64_t id = symbol_to_id.at(symbol);
            if (id_to_type.at(id) != type)
                throw std::invalid_argument(
                    "Type exists already for symbol \"" + symbol + "\"");
            return id;
        }

        uint64_t id = symbol_to_id.size();
        symbol_to_id[symbol] = id;
        id_to_symbol[id] = symbol;
        id_to_type[id] = type;
        return id;
    }

    uint64_t SymbolTable::getId(std::string symbol) {
        return symbol_to_id.at(symbol);
    }

    std::string SymbolTable::getSymbol(uint64_t id) {
        return id_to_symbol.at(id);
    }

    Type SymbolTable::getType(uint64_t id) { return id_to_type.at(id); }

    bool SymbolTable::hasSymbol(std::string symbol) {
        return symbol_to_id.contains(symbol);
    }

    std::string SymbolTable::toString() {
        std::string res = "";

        for (const auto [id, symbol] : id_to_symbol) {
            auto type = id_to_type.at(id);
            res += "  " + std::to_string(id) + ": (" +
                   ::frontend::toString(type) + ") " + symbol + "\n";
        }

        return res;
    }
} // namespace frontend
