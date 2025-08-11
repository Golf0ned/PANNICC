#pragma once

namespace middleend::mir {
    class InstructionVisitor;

    class Instruction {
        public:
            virtual void accept(InstructionVisitor* visitor);
            virtual ~Instruction() = default;
    };

    // TODO: standard instructions

    class Terminator : Instruction {
        public:
            virtual void accept(InstructionVisitor* visitor);
            virtual ~Terminator() = default;
    };

    // TODO: terminators
}

