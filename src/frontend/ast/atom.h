#pragma once

#include <cstdint>
#include <string>


namespace frontend::ast {
    class AtomVisitor;

    class Atom {
        public:
            virtual void accept(AtomVisitor* visitor) = 0;
            virtual ~Atom() = default;
    };

    class AtomIdentifier : public Atom {
        public:
            AtomIdentifier(std::string identifier);
            std::string getValue();
            void accept(AtomVisitor* visitor) override;

        private:
            std::string value;
    };

    class AtomLiteral : public Atom {
        public:
            AtomLiteral(std::string value);
            uint64_t getValue();
            void accept(AtomVisitor* visitor) override;

        private:
            uint64_t value;
    };

    class AtomVisitor {
        public:
            virtual void visit(Atom* a) = 0;
            virtual void visit(AtomIdentifier* a) = 0;
            virtual void visit(AtomLiteral* a) = 0;
    };
}
