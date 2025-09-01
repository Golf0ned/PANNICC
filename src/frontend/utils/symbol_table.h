#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace frontend {
    std::string toScopedSymbol(std::string symbol, uint64_t scope);

    class SymbolTable {
    public:
        uint64_t addSymbol(std::string symbol);
        std::string getSymbol(uint64_t id);
        bool hasSymbol(std::string symbol);
        std::string toString();

    private:
        std::unordered_map<uint64_t, std::string> id_to_symbol;
        std::unordered_map<std::string, uint64_t> symbol_to_id;
    };
} // namespace frontend
