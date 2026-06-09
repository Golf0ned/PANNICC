#pragma once

#include "middleend/mir/instruction.h"
#include "middleend/mir/type.h"

#include <cstdint>
#include <list>
#include <unordered_set>

namespace middleend {

class NumberIR;

namespace mir {

class BasicBlockEdges {
public:
    BasicBlockEdges();
    const std::vector<BasicBlock *> getEdges();              // TODO: iterator
    const std::unordered_set<BasicBlock *> getUniqueEdges(); // TODO: iterator
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
    Terminator *getTerminator();
    BasicBlockEdges &getPredecessors();
    BasicBlockEdges &getSuccessors();
    void setTerminator(std::unique_ptr<Terminator> new_terminator);
    std::string toString(NumberIR *nir);

private:
    std::list<std::unique_ptr<Instruction>> body;
    std::unique_ptr<Terminator> terminator;
    BasicBlockEdges predecessors;
    BasicBlockEdges successors;
};

class Function {
public:
    virtual Type getType() = 0;
    virtual std::string getName() = 0;
    virtual std::string toString() = 0;
    virtual ~Function() = default;
};

class FunctionDefinition : public Function {
public:
    FunctionDefinition(Type type, std::string name,
                       std::vector<std::unique_ptr<Value>> parameters,
                       std::list<std::unique_ptr<BasicBlock>> basic_blocks,
                       BasicBlock *entry_block);
    Type getType() override;
    std::string getName() override;
    std::vector<std::unique_ptr<Value>> &getParameters();
    std::list<std::unique_ptr<BasicBlock>> &getBasicBlocks();
    BasicBlock *getEntryBlock();
    void setEntryBlock(BasicBlock *new_block);
    std::string toString() override;

private:
    Type type;
    std::string name;
    std::vector<std::unique_ptr<Value>> parameters;
    std::list<std::unique_ptr<BasicBlock>> basic_blocks;
    BasicBlock *entry_block;
};

class FunctionDeclaration : public Function {
public:
    FunctionDeclaration(Type type, std::string name,
                        std::vector<std::unique_ptr<Value>> parameters);
    Type getType() override;
    std::string getName() override;
    std::vector<std::unique_ptr<Value>> &getParameters();
    std::string toString() override;

private:
    Type type;
    std::string name;
    std::vector<std::unique_ptr<Value>> parameters;
};

class Program {
public:
    Program(std::list<std::unique_ptr<Function>> functions);
    std::list<std::unique_ptr<Function>> &getFunctions();
    std::string toString();

private:
    std::list<std::unique_ptr<Function>> functions;
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
    void visit(InstructionParallelCopy *i) override;

    void visit(TerminatorReturn *t) override;
    void visit(TerminatorBranch *t) override;
    void visit(TerminatorCondBranch *t) override;

private:
    std::string result;
    NumberIR *nir;
};

} // namespace mir

} // namespace middleend
