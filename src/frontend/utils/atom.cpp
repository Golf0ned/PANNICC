#include "frontend/utils/atom.h"
#include "frontend/utils/symbol_table.h"

namespace frontend {
    AtomIdentifier::AtomIdentifier(uint64_t value, Type type)
        : value(value), type(type) {}

    bool AtomIdentifier::isIdentifier() { return true; }

    uint64_t AtomIdentifier::getValue() { return value; }

    Type AtomIdentifier::getType() { return type; }

    std::string AtomIdentifier::toString(SymbolTable &symbol_table) {
        return symbol_table.getSymbol(value);
    }

    AtomLiteral::AtomLiteral(uint64_t value, Type type)
        : value(value), type(type) {}

    bool AtomLiteral::isIdentifier() { return false; }

    uint64_t AtomLiteral::getValue() { return value; }

    Type AtomLiteral::getType() { return type; }

    std::string AtomLiteral::toString(SymbolTable &symbol_table) {
        return std::to_string(value);
    }
} // namespace frontend
