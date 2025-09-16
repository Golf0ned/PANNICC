#pragma once

#include <memory>

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"

namespace frontend::hir {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor) = 0;
        virtual ~Instruction() = default;
    };

    class InstructionDeclaration : public Instruction {
    public:
        InstructionDeclaration(Type type,
                               std::unique_ptr<AtomIdentifier> variable);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getVariable();
        void accept(InstructionVisitor *visitor);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> variable;
    };

    class InstructionAssignValue : public Instruction {
    public:
        InstructionAssignValue(std::unique_ptr<AtomIdentifier> variable,
                               std::unique_ptr<Atom> value);
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<Atom> &getValue();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<Atom> value;
    };

    class InstructionAssignBinaryOp : public Instruction {
    public:
        InstructionAssignBinaryOp(std::unique_ptr<AtomIdentifier> variable,
                                  BinaryOp op, std::unique_ptr<Atom> left,
                                  std::unique_ptr<Atom> right);
        std::unique_ptr<AtomIdentifier> &getVariable();
        BinaryOp getOp();
        std::unique_ptr<Atom> &getLeft();
        std::unique_ptr<Atom> &getRight();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        BinaryOp op;
        std::unique_ptr<Atom> left;
        std::unique_ptr<Atom> right;
    };

    class InstructionReturn : public Instruction {
    public:
        InstructionReturn(std::unique_ptr<Atom> value);
        std::unique_ptr<Atom> &getValue();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<Atom> value;
    };

    class InstructionCall : public Instruction {
    public:
        InstructionCall(std::unique_ptr<AtomIdentifier> callee);
        std::unique_ptr<AtomIdentifier> &getCallee();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> callee;
    };

    class InstructionCallAssign : public Instruction {
    public:
        InstructionCallAssign(std::unique_ptr<AtomIdentifier> variable,
                              std::unique_ptr<AtomIdentifier> callee);
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<AtomIdentifier> &getCallee();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<AtomIdentifier> callee;
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(InstructionDeclaration *i) = 0;
        virtual void visit(InstructionAssignValue *i) = 0;
        virtual void visit(InstructionAssignBinaryOp *i) = 0;
        virtual void visit(InstructionReturn *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionCallAssign *i) = 0;
    };
} // namespace frontend::hir
