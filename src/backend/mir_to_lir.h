#pragma once

#include "backend/lir/lir.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/operator.h"

namespace backend {
    lir::Program mirToLir(middleend::mir::Program &mir);

    class Node {};

    class RegisterNode : public Node {
    public:
        RegisterNode();

    private:
        std::string name;
        Node *source;
    };
    class ImmediateNode : public Node {
    public:
    private:
        uint64_t value;
    };
    class OpNode : public Node {
    public:
    private:
        middleend::mir::BinaryOp op;
        Node *left;
        Node *right;
    };

    class TreeGenVisitor : public middleend::mir::InstructionVisitor {
    public:
        virtual void visit(middleend::mir::InstructionBinaryOp *i);
        virtual void visit(middleend::mir::InstructionCall *i);
        virtual void visit(middleend::mir::InstructionAlloca *i);
        virtual void visit(middleend::mir::InstructionLoad *i);
        virtual void visit(middleend::mir::InstructionStore *i);
        virtual void visit(middleend::mir::InstructionPhi *i);

        virtual void visit(middleend::mir::TerminatorReturn *t);
        virtual void visit(middleend::mir::TerminatorBranch *t);
        virtual void visit(middleend::mir::TerminatorCondBranch *t);

    private:
    };
} // namespace backend
