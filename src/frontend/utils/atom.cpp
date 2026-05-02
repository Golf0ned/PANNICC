#include "frontend/utils/atom.h"

using namespace frontend;

AtomIdentifier::AtomIdentifier(uint64_t value) : value(value) {}

bool AtomIdentifier::isIdentifier() { return true; }

uint64_t AtomIdentifier::getValue() { return value; }

std::string AtomIdentifier::toString(SymbolTable &st) {
    return st.getSymbol(value);
}

AtomLiteral::AtomLiteral(uint64_t value) : value(value) {}

bool AtomLiteral::isIdentifier() { return false; }

uint64_t AtomLiteral::getValue() { return value; }

std::string AtomLiteral::toString(SymbolTable &st) {
    return std::to_string(value);
}
