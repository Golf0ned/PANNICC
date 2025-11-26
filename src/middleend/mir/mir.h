#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

namespace middleend {
    class NumberIR;
}

namespace middleend::mir {
    using LiteralMap = std::unordered_map<
        Type, std::unordered_map<uint64_t, std::unique_ptr<Literal>>>;

    class BasicBlockEdges {
    public:
        BasicBlockEdges();
        const std::vector<BasicBlock *> getEdges(); // TODO: iterator
        const std::unordered_set<BasicBlock *>
        getUniqueEdges(); // TODO: iterator
        void addEdge(BasicBlock *bb);
        void removeEdge(BasicBlock *bb);
        uint64_t getSize();

    private:
        std::unordered_map<BasicBlock *, uint64_t> edges;
        uint64_t size;
    };

    class BasicBlock {
    public:
        BasicBlock(std::list<std::unique_ptr<Instruction>> body,
                   std::unique_ptr<Terminator> terminator);
        std::list<std::unique_ptr<Instruction>> &getInstructions();
        std::unique_ptr<Terminator> &getTerminator();
        BasicBlockEdges &getPredecessors();
        BasicBlockEdges &getSuccessors();
        std::string toString(NumberIR *nir);

    private:
        std::list<std::unique_ptr<Instruction>> body;
        std::unique_ptr<Terminator> terminator;
        BasicBlockEdges predecessors;
        BasicBlockEdges successors;
    };

    class Function {
    public:
        Function(Type type, std::string name,
                 std::vector<std::unique_ptr<Value>> parameters,
                 std::list<std::unique_ptr<BasicBlock>> basic_blocks,
                 BasicBlock *entry_block);
        Type getType();
        std::string getName();
        std::vector<std::unique_ptr<Value>> &getParameters();
        std::list<std::unique_ptr<BasicBlock>> &getBasicBlocks();
        BasicBlock *getEntryBlock();
        void setEntryBlock(BasicBlock *new_block);
        std::string toString();

    private:
        Type type;
        std::string name;
        std::vector<std::unique_ptr<Value>> parameters;
        std::list<std::unique_ptr<BasicBlock>> basic_blocks;
        BasicBlock *entry_block;
    };

    class Program {
    public:
        Program(std::list<std::unique_ptr<Function>> functions,
                LiteralMap literals);
        std::list<std::unique_ptr<Function>> &getFunctions();
        Literal *getLiteral(Type type, uint64_t value);
        std::string toString();

    private:
        std::list<std::unique_ptr<Function>> functions;
        LiteralMap literals;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        ToStringVisitor(NumberIR *nir);
        std::string getResult();

        std::string valueToString(Value *v);
        std::string valueToTypedString(Value *v);

        void visit(InstructionBinaryOp *i) override;
        void visit(InstructionCall *i) override;
        void visit(InstructionAlloca *i) override;
        void visit(InstructionLoad *i) override;
        void visit(InstructionStore *i) override;
        void visit(InstructionPhi *i) override;

        void visit(TerminatorReturn *t) override;
        void visit(TerminatorBranch *t) override;
        void visit(TerminatorCondBranch *t) override;

    private:
        std::string result;
        NumberIR *nir;
    };
} // namespace middleend::mir
