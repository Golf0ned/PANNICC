#include <vector>

#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/type.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    BasicBlock::BasicBlock(std::vector<std::unique_ptr<Instruction>> body,
                           std::unique_ptr<Terminator> terminator,
                           std::vector<std::unique_ptr<Literal>> literals)
        : body(std::move(body)), terminator(std::move(terminator)),
          literals(std::move(literals)) {}

    std::string BasicBlock::toString(uint64_t &counter, bool isEntry) {
        ToStringVisitor visitor(counter);

        std::string name =
            isEntry ? "entry" : std::to_string(visitor.getNextIdentifier());
        std::string res = name + ":\n";

        for (auto &i : body) {
            i->accept(&visitor);
            res += "  " + visitor.getResult() + '\n';
        }

        terminator->accept(&visitor);
        res += "  " + visitor.getResult();

        return res;
    }

    Function::Function(Type type, std::string name,
                       std::vector<BasicBlock> basic_blocks)
        : type(type), name(name), basic_blocks(std::move(basic_blocks)) {}

    Type Function::getType() { return type; }

    std::string Function::getName() { return name; }

    std::string Function::toString() {
        // TODO: function params
        std::string res = "define " + ::middleend::mir::toString(type) + " @" +
                          name + "() {\n";

        uint64_t counter = 0;
        res += basic_blocks[0].toString(counter, true);
        for (auto iter = ++basic_blocks.begin(); iter != basic_blocks.end();
             iter++) {
            res += "\n\n" + iter->toString(counter);
        }
        res += "\n}";
        return res;
    }

    Program::Program(std::vector<Function> functions)
        : functions(std::move(functions)) {}

    std::string Program::toString() {
        if (functions.empty())
            return "";

        std::string res = functions[0].toString();
        for (auto iter = ++functions.begin(); iter != functions.end(); iter++)
            res += "\n\n" + iter->toString();

        return res;
    }

    ToStringVisitor::ToStringVisitor(uint64_t &counter)
        : result(), counter(counter) {}

    std::string ToStringVisitor::getResult() { return result; }

    uint64_t ToStringVisitor::resolveBasicBlock(BasicBlock *basic_block) {
        auto iter = basic_block_ids.find(basic_block);
        if (iter != basic_block_ids.end())
            return iter->second;

        uint64_t new_id = counter++;
        basic_block_ids[basic_block] = new_id;
        return new_id;
    }

    uint64_t ToStringVisitor::resolveInstruction(Instruction *instruction) {
        auto iter = instruction_ids.find(instruction);
        if (iter != instruction_ids.end())
            return iter->second;

        uint64_t new_id = counter++;
        instruction_ids[instruction] = new_id;
        return new_id;
    }

    uint64_t ToStringVisitor::getNextIdentifier() { return counter++; }

    std::string ToStringVisitor::valueToString(Value *v) {
        std::string v_type = toString(v->getType());
        Literal *l = dynamic_cast<Literal *>(v);
        if (l)
            return v_type + ' ' + std::to_string(l->getValue());
        // TODO: function params
        return v_type + " %" +
               std::to_string(
                   resolveInstruction(reinterpret_cast<Instruction *>(v)));
    }

    void ToStringVisitor::visit(InstructionBinaryOp *i) {
        std::string var = valueToString(i);
        std::string op = toString(i->getOp());
        std::string left = valueToString(i->getLeft());
        std::string right = valueToString(i->getRight());

        result = var + " = " + op + ' ' + left + ", " + right;
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        std::string var = valueToString(i);
        std::string f_type = toString(i->getCallee()->getType());
        std::string f_name = i->getCallee()->getName();

        result = var + " = call " + f_type + ' ' + f_name;
    }
    void ToStringVisitor::visit(InstructionAlloca *i) {
        std::string var = valueToString(i);
        std::string alloc_type = toString(i->getAllocType());

        result = var + " = alloca " + alloc_type;
    }
    void ToStringVisitor::visit(InstructionLoad *i) {
        std::string var = valueToString(i);
        std::string ptr = valueToString(i->getPtr());

        result = var + " = load " + ptr;
    }
    void ToStringVisitor::visit(InstructionStore *i) {
        std::string value = valueToString(i->getValue());
        std::string ptr = valueToString(i->getPtr());

        result = "store " + value + ", " + ptr;
    }

    void ToStringVisitor::visit(TerminatorReturn *t) {
        std::string value = valueToString(t->getValue());

        result = "ret " + value;
    }
} // namespace middleend::mir
