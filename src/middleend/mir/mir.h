#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <unordered_set>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

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
        std::string toString(
            const std::unordered_map<BasicBlock *, uint64_t> &basic_block_ids,
            const std::unordered_map<Value *, uint64_t> &instruction_ids);

    private:
        std::list<std::unique_ptr<Instruction>> body;
        std::unique_ptr<Terminator> terminator;
        BasicBlockEdges predecessors;
        BasicBlockEdges successors;
    };

    class Function {
    public:
        Function(Type type, std::string name,
                 std::vector<std::unique_ptr<Value *>> parameters,
                 std::list<std::unique_ptr<BasicBlock>> basic_blocks,
                 BasicBlock *entry_block);
        Type getType();
        std::string getName();
        std::vector<std::unique_ptr<Value *>> &getParameters();
        std::list<std::unique_ptr<BasicBlock>> &getBasicBlocks();
        BasicBlock *getEntryBlock();
        void setEntryBlock(BasicBlock *new_block);
        std::string toString();

    private:
        Type type;
        std::string name;
        std::vector<std::unique_ptr<Value *>> parameters;
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
        ToStringVisitor(
            const std::unordered_map<BasicBlock *, uint64_t> &basic_block_ids,
            const std::unordered_map<Value *, uint64_t> &instruction_ids);
        std::string getResult();

        std::string valueToString(Value *v);
        std::string valueToTypedString(Value *v);

        virtual void visit(InstructionBinaryOp *i);
        virtual void visit(InstructionCall *i);
        virtual void visit(InstructionAlloca *i);
        virtual void visit(InstructionLoad *i);
        virtual void visit(InstructionStore *i);
        virtual void visit(InstructionPhi *i);

        virtual void visit(TerminatorReturn *t);
        virtual void visit(TerminatorBranch *t);
        virtual void visit(TerminatorCondBranch *t);

    private:
        std::string result;
        std::unordered_map<BasicBlock *, uint64_t> basic_block_ids;
        std::unordered_map<Value *, uint64_t> instruction_ids;
    };
} // namespace middleend::mir
