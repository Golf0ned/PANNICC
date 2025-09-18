#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

namespace middleend::mir {

    class BasicBlock {
    public:
        BasicBlock(std::vector<std::unique_ptr<Instruction>> body,
                   std::unique_ptr<Terminator> terminator,
                   std::vector<std::unique_ptr<Literal>> literals);
        std::vector<std::unique_ptr<Instruction>> &getInstructions();
        std::vector<BasicBlock *> &getPredecessors();
        std::vector<BasicBlock *> &getSuccessors();
        std::string toString(uint64_t &counter, bool isEntry = false);

    private:
        std::vector<std::unique_ptr<Instruction>> body;
        std::vector<std::unique_ptr<Literal>> literals;
        std::unique_ptr<Terminator> terminator;
        std::vector<BasicBlock *> predecessors;
        std::vector<BasicBlock *> successors;
    };

    class Function {
    public:
        Function(Type type, std::string name,
                 std::vector<BasicBlock> basic_blocks);
        Type getType();
        std::string getName();
        std::vector<BasicBlock> &getBasicBlocks();
        std::string toString();

    private:
        Type type;
        std::string name;
        std::vector<BasicBlock> basic_blocks;
    };

    class Program {
    public:
        Program(std::vector<Function> functions);
        std::vector<Function> &getFunctions();
        std::string toString();

    private:
        std::vector<Function> functions;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        ToStringVisitor(uint64_t &counter);
        std::string getResult();

        uint64_t resolveBasicBlock(BasicBlock *basic_block);
        uint64_t resolveInstruction(Instruction *instruction);
        uint64_t getNextIdentifier();
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
        uint64_t &counter;
        std::unordered_map<BasicBlock *, uint64_t> basic_block_ids;
        std::unordered_map<Instruction *, uint64_t> instruction_ids;
    };
} // namespace middleend::mir
