#include "frontend/ast/atom.h"
#include <string>


namespace frontend::AST {
    AtomIdentifier::AtomIdentifier(std::string value) {
        this->value = value;
    }

    std::string AtomIdentifier::getValue() {
        return value;
    }

    void AtomIdentifier::accept(AtomVisitor* visitor) {
        visitor->visit(this);
    }


    AtomLiteral::AtomLiteral(std::string value) {
        this->value = std::stoull(value);
    }

    uint64_t AtomLiteral::getValue() {
        return value;
    }

    void AtomLiteral::accept(AtomVisitor* visitor) {
        visitor->visit(this);
    }
}
