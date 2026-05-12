#pragma once

#include "frontend/utils/symbol_table.h"

#include <cstdint>
#include <string>

namespace frontend {

class Atom {
public:
    virtual bool isIdentifier() = 0;
    virtual uint64_t getValue() = 0;
    virtual std::string toString(SymbolTable &st) = 0;
    virtual ~Atom() = default;
};

class AtomIdentifier : public Atom {
public:
    AtomIdentifier(uint64_t value);
    bool isIdentifier() override;
    uint64_t getValue() override;
    std::string toString(SymbolTable &st) override;

private:
    uint64_t value;
};

class AtomLiteral : public Atom {
public:
    AtomLiteral(uint64_t value);
    bool isIdentifier() override;
    uint64_t getValue() override;
    std::string toString(SymbolTable &st) override;

private:
    uint64_t value;
};

} // namespace frontend
