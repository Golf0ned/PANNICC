#pragma once

#include <cstdint>


namespace frontend{
    class Atom {
        public:
            virtual bool isIdentifier() = 0;
            virtual uint64_t getValue() = 0;
    };

    class AtomIdentifier : public Atom {
        public:
            AtomIdentifier(uint64_t value);
            bool isIdentifier() override;
            uint64_t getValue() override;
        private:
            uint64_t value;
    };

    class AtomLiteral : public Atom {
        public:
            AtomLiteral(uint64_t value);
            bool isIdentifier() override;
            uint64_t getValue() override;
        private:
            uint64_t value;
    };
}
