#include "frontend/utils/atom.h"


namespace frontend {
    AtomIdentifier::AtomIdentifier(uint64_t value)
        : value(value) {}

    bool AtomIdentifier::isIdentifier() {
        return true;
    }

    uint64_t AtomIdentifier::getValue() {
        return value;
    }


    AtomLiteral::AtomLiteral(uint64_t value)
        : value(value) {}

    bool AtomLiteral::isIdentifier() {
        return false;
    }

    uint64_t AtomLiteral::getValue() {
        return value;
    }
}
