#pragma once

#include <memory>
#include <vector>

#include "frontend/utils/atom.h"
#include "frontend/utils/operator.h"
#include "frontend/utils/type.h"

namespace frontend::ast {
    class InstructionVisitor;

    class Instruction {
    public:
        virtual void accept(InstructionVisitor *visitor) = 0;
        virtual ~Instruction() = default;
    };

    class Scope : public Instruction {
    public:
        void addInstruction(std::unique_ptr<Instruction> i);
        std::vector<std::unique_ptr<Instruction>> &getInstructions();
        void accept(InstructionVisitor *visitor);

    private:
        std::vector<std::unique_ptr<Instruction>> instructions;
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

    class InstructionDeclarationAssignValue : public Instruction {
    public:
        InstructionDeclarationAssignValue(
            Type type, std::unique_ptr<AtomIdentifier> variable,
            std::unique_ptr<Atom> value);
        Type getType();
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<Atom> &getValue();
        void accept(InstructionVisitor *visitor);

    private:
        Type type;
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<Atom> value;
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
        InstructionCall(std::unique_ptr<AtomIdentifier> target);
        std::unique_ptr<AtomIdentifier> &getTarget();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> target;
    };

    class InstructionCallAssign : public Instruction {
    public:
        InstructionCallAssign(std::unique_ptr<AtomIdentifier> variable,
                              std::unique_ptr<AtomIdentifier> target);
        std::unique_ptr<AtomIdentifier> &getVariable();
        std::unique_ptr<AtomIdentifier> &getTarget();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<AtomIdentifier> variable;
        std::unique_ptr<AtomIdentifier> target;
    };

    class InstructionIf : public Instruction {
    public:
        InstructionIf(std::unique_ptr<Atom> cond,
                      std::unique_ptr<Instruction> t_branch,
                      std::unique_ptr<Instruction> f_branch);
        std::unique_ptr<Atom> &getCond();
        std::unique_ptr<Instruction> &getTBranch();
        std::unique_ptr<Instruction> &getFBranch();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<Atom> cond;
        std::unique_ptr<Instruction> t_branch;
        std::unique_ptr<Instruction> f_branch;
    };

    class InstructionWhile : public Instruction {
    public:
        InstructionWhile(std::unique_ptr<Atom> cond,
                         std::unique_ptr<Instruction> body);
        std::unique_ptr<Atom> &getCond();
        std::unique_ptr<Instruction> &getBody();
        void accept(InstructionVisitor *visitor);

    private:
        std::unique_ptr<Atom> cond;
        std::unique_ptr<Instruction> body;
    };

    class InstructionVisitor {
    public:
        virtual void visit(Instruction *i) = 0;
        virtual void visit(Scope *s) = 0;
        virtual void visit(InstructionDeclaration *i) = 0;
        virtual void visit(InstructionDeclarationAssignValue *i) = 0;
        virtual void visit(InstructionAssignValue *i) = 0;
        virtual void visit(InstructionAssignBinaryOp *i) = 0;
        virtual void visit(InstructionReturn *i) = 0;
        virtual void visit(InstructionCall *i) = 0;
        virtual void visit(InstructionCallAssign *i) = 0;
        virtual void visit(InstructionIf *i) = 0;
        virtual void visit(InstructionWhile *i) = 0;
    };
} // namespace frontend::ast
