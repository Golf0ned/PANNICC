#pragma once

#include <cstdint>


namespace frontend::ast {
    class AtomVisitor;

    class Atom {
        public:
            virtual bool isIdentifier() = 0;
            virtual uint64_t getValue() = 0;
            virtual void accept(AtomVisitor* visitor) = 0;
            virtual ~Atom() = default;
    };

    class AtomIdentifier : public Atom {
        public:
            AtomIdentifier(uint64_t value);
            bool isIdentifier() override;
            uint64_t getValue() override;
            void accept(AtomVisitor* visitor) override;
        private:
            bool identifier;
            uint64_t value;
    };

    class AtomLiteral : public Atom {
        public:
            AtomLiteral(uint64_t value);
            bool isIdentifier() override;
            uint64_t getValue() override;
            void accept(AtomVisitor* visitor) override;
        private:
            bool identifier;
            uint64_t value;
    };

    class AtomVisitor {
        public:
            virtual void visit(Atom* a) = 0;
            virtual void visit(AtomIdentifier* a) = 0;
            virtual void visit(AtomLiteral* a) = 0;
    };
}
