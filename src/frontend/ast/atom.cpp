#include "frontend/ast/atom.h"


namespace frontend::ast {
    AtomIdentifier::AtomIdentifier(uint64_t value)
        : value(value), identifier(true) {}

    bool AtomIdentifier::isIdentifier() {
        return identifier;
    }

    uint64_t AtomIdentifier::getValue() {
        return value;
    }

    void AtomIdentifier::accept(AtomVisitor* visitor) {
        visitor->visit(this);
    }


    AtomLiteral::AtomLiteral(uint64_t value)
        : value(value), identifier(false) {}

    bool AtomLiteral::isIdentifier() {
        return identifier;
    }

    uint64_t AtomLiteral::getValue() {
        return value;
    }

    void AtomLiteral::accept(AtomVisitor* visitor) {
        visitor->visit(this);
    }
}
