#pragma once

#include <cstdint>
#include <string>

#include "frontend/utils/symbol_table.h"
#include "frontend/utils/type.h"

namespace frontend {
    class Atom {
    public:
        virtual bool isIdentifier() = 0;
        virtual uint64_t getValue() = 0;
        virtual Type getType() = 0;
        virtual std::string toString(SymbolTable &symbol_table) = 0;
        virtual ~Atom() = default;
    };

    class AtomIdentifier : public Atom {
    public:
        AtomIdentifier(uint64_t value, Type type);
        bool isIdentifier() override;
        uint64_t getValue() override;
        Type getType() override;
        std::string toString(SymbolTable &symbol_table) override;

    private:
        uint64_t value;
        Type type;
    };

    class AtomLiteral : public Atom {
    public:
        AtomLiteral(uint64_t value, Type type);
        bool isIdentifier() override;
        uint64_t getValue() override;
        Type getType() override;
        std::string toString(SymbolTable &symbol_table) override;

    private:
        uint64_t value;
        Type type;
    };
} // namespace frontend
