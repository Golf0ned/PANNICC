#pragma once

#include <cstdint>
#include <string>

#include "frontend/utils/symbol_table.h"

namespace frontend {
    class Atom {
    public:
        virtual bool isIdentifier() = 0;
        virtual uint64_t getValue() = 0;
        virtual std::string toString(SymbolTable &symbol_table) = 0;
        virtual ~Atom() = default;
    };

    class AtomIdentifier : public Atom {
    public:
        AtomIdentifier(uint64_t value);
        bool isIdentifier() override;
        uint64_t getValue() override;
        std::string toString(SymbolTable &symbol_table) override;

    private:
        uint64_t value;
    };

    class AtomLiteral : public Atom {
    public:
        AtomLiteral(uint64_t value);
        bool isIdentifier() override;
        uint64_t getValue() override;
        std::string toString(SymbolTable &symbol_table) override;

    private:
        uint64_t value;
    };
} // namespace frontend
