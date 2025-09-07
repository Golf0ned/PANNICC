#pragma once

#include <cstdint>
#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

namespace middleend::mir {

    class BasicBlock {
    public:
        BasicBlock(std::vector<Instruction *> body, Terminator *terminator);
        std::string toString(bool isEntry = false);
        ~BasicBlock();

    private:
        std::vector<Instruction *> body;
        Terminator *terminator;
    };

    class Function {
    public:
        Function(Type type, std::string name,
                 std::vector<BasicBlock> basic_blocks);
        Type getType();
        std::string getName();
        std::string toString();

    private:
        Type type;
        std::string name;
        std::vector<BasicBlock> basic_blocks;
    };

    class Program {
    public:
        Program(std::vector<Function> functions);
        std::string toString();

    private:
        std::vector<Function> functions;
    };

    class ToStringVisitor : public InstructionVisitor {
    public:
        ToStringVisitor() = default;
        std::string getResult();

        uint64_t resolveBasicBlock(BasicBlock *basic_block);
        uint64_t resolveInstruction(Instruction *instruction);
        uint64_t getNextIdentifier();
        std::string valueToString(Value *v);

        virtual void visit(InstructionBinaryOp *i);
        virtual void visit(InstructionCall *i);
        virtual void visit(InstructionAlloca *i);
        virtual void visit(InstructionLoad *i);
        virtual void visit(InstructionStore *i);

        virtual void visit(TerminatorReturn *t);

    private:
        std::string result;
        uint64_t counter;
        std::unordered_map<BasicBlock *, uint64_t> basic_block_ids;
        std::unordered_map<Instruction *, uint64_t> instruction_ids;
    };
} // namespace middleend::mir
