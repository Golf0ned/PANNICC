#include <map>

#include "middleend/analysis/number_ir.h"
#include "middleend/mir/instruction.h"
#include "middleend/mir/mir.h"
#include "middleend/mir/type.h"
#include "middleend/mir/value.h"

namespace middleend::mir {
    BasicBlockEdges::BasicBlockEdges() : size(0) {}

    const std::vector<BasicBlock *> BasicBlockEdges::getEdges() {
        std::vector<BasicBlock *> res;
        for (auto &[bb, count] : edges)
            for (int i = 0; i < count; i++)
                res.push_back(bb);
        return res;
    }

    const std::unordered_set<BasicBlock *> BasicBlockEdges::getUniqueEdges() {
        std::unordered_set<BasicBlock *> res;
        for (auto &[bb, _] : edges)
            res.insert(bb);
        return res;
    }

    void BasicBlockEdges::addEdge(BasicBlock *bb) {
        edges[bb]++;
        size++;
    }

    void BasicBlockEdges::removeEdge(BasicBlock *bb) {
        edges.at(bb); // too lazy to add an exception by hand lmao
        if (--edges[bb] == 0)
            edges.erase(bb);
        size--;
    }

    uint64_t BasicBlockEdges::getSize() { return size; }

    BasicBlock::BasicBlock(std::list<std::unique_ptr<Instruction>> body,
                           std::unique_ptr<Terminator> terminator)
        : body(std::move(body)), terminator(std::move(terminator)) {}

    std::list<std::unique_ptr<Instruction>> &BasicBlock::getInstructions() {
        return body;
    }

    std::unique_ptr<Terminator> &BasicBlock::getTerminator() {
        return terminator;
    }

    BasicBlockEdges &BasicBlock::getPredecessors() { return predecessors; }

    BasicBlockEdges &BasicBlock::getSuccessors() { return successors; }

    std::string BasicBlock::toString(NumberIR *nir) {
        uint64_t block_id = nir->getNumber(this);
        std::string name = block_id == -1 ? "entry" : std::to_string(block_id);

        std::string res = name + ":\n";

        ToStringVisitor tsv(nir);
        for (auto &i : body) {
            i->accept(&tsv);
            res += "  " + tsv.getResult() + '\n';
        }

        terminator->accept(&tsv);
        res += "  " + tsv.getResult();

        return res;
    }

    Function::Function(Type type, std::string name,
                       std::vector<std::unique_ptr<Value>> parameters,
                       std::list<std::unique_ptr<BasicBlock>> basic_blocks,
                       BasicBlock *entry_block)
        : type(type), name(name), parameters(std::move(parameters)),
          basic_blocks(std::move(basic_blocks)), entry_block(entry_block) {}

    Type Function::getType() { return type; }

    std::string Function::getName() { return name; }

    std::vector<std::unique_ptr<Value>> &Function::getParameters() {
        return parameters;
    }

    std::list<std::unique_ptr<BasicBlock>> &Function::getBasicBlocks() {
        return basic_blocks;
    }

    BasicBlock *Function::getEntryBlock() { return entry_block; }

    void Function::setEntryBlock(BasicBlock *new_block) {
        entry_block = new_block;
    }

    std::string Function::toString() {
        NumberIR nir;
        nir.run(this);

        std::string res =
            "define " + ::middleend::mir::toString(type) + " @" + name + "(";

        ToStringVisitor tsv(&nir);
        for (auto iter = parameters.begin(); iter != parameters.end(); iter++) {
            if (iter != parameters.begin())
                res += ", ";
            res += tsv.valueToTypedString(iter->get());
        }

        res += ") {\n";

        for (auto iter = basic_blocks.begin(); iter != basic_blocks.end();
             iter++) {
            if (iter != basic_blocks.begin())
                res += "\n\n";
            res += iter->get()->toString(&nir);
        }
        res += "\n}";
        return res;
    }

    Program::Program(std::list<std::unique_ptr<Function>> functions,
                     LiteralMap literals)
        : functions(std::move(functions)), literals(std::move(literals)) {}

    std::list<std::unique_ptr<Function>> &Program::getFunctions() {
        return functions;
    }

    Literal *Program::getLiteral(Type type, uint64_t value) {
        auto &typed_map = literals[type];
        if (typed_map.contains(value))
            return typed_map.at(value).get();

        auto literal = std::make_unique<Literal>(type, value);
        auto literal_ptr = literal.get();

        typed_map[value] = std::move(literal);
        return literal_ptr;
    }

    std::string Program::toString() {
        std::string res = "";
        for (auto iter = functions.begin(); iter != functions.end(); iter++) {
            if (iter != functions.begin())
                res += "\n\n";
            res += iter->get()->toString();
        }

        return res;
    }

    ToStringVisitor::ToStringVisitor(NumberIR *nir) : nir(nir) {}

    std::string ToStringVisitor::getResult() { return result; }

    std::string ToStringVisitor::valueToString(Value *v) {
        std::string v_type = toString(v->getType());
        Literal *l = dynamic_cast<Literal *>(v);
        if (l)
            return std::to_string(l->getValue());
        return "%" + std::to_string(nir->getNumber(v));
    }

    std::string ToStringVisitor::valueToTypedString(Value *v) {
        return toString(v->getType()) + ' ' + valueToString(v);
    }

    void ToStringVisitor::visit(InstructionBinaryOp *i) {
        std::string var = valueToString(i);
        std::string op = toString(i->getOp());
        std::string left = valueToTypedString(i->getLeft());
        std::string right = valueToTypedString(i->getRight());

        result = var + " = " + op + ' ' + left + ", " + right;
    }

    void ToStringVisitor::visit(InstructionCall *i) {
        std::string var = valueToString(i);
        std::string f_type = toString(i->getCallee()->getType());
        std::string f_name = i->getCallee()->getName();

        result = var + " = call " + f_type + " @" + f_name + '(';

        auto &args = i->getArguments();
        for (auto iter = args.begin(); iter != args.end(); iter++) {
            if (iter != args.begin())
                result += ", ";
            result += valueToTypedString(*iter);
        }

        result += ')';
    }

    void ToStringVisitor::visit(InstructionAlloca *i) {
        std::string var = valueToString(i);
        std::string alloc_type = toString(i->getAllocType());

        result = var + " = alloca " + alloc_type;
    }

    void ToStringVisitor::visit(InstructionLoad *i) {
        std::string var = valueToString(i);
        std::string ptr = valueToTypedString(i->getPtr());

        result = var + " = load " + ptr;
    }

    void ToStringVisitor::visit(InstructionStore *i) {
        std::string value = valueToTypedString(i->getValue());
        std::string ptr = valueToTypedString(i->getPtr());

        result = "store " + value + ", " + ptr;
    }

    void ToStringVisitor::visit(InstructionPhi *i) {
        std::string var = valueToString(i);
        std::string type = toString(i->getType());

        auto comp = [&](BasicBlock *a, BasicBlock *b) {
            return nir->getNumber(a) < nir->getNumber(b);
        };

        auto pairs = i->getPredecessors();
        std::map<BasicBlock *, Value *, decltype(comp)> ordered(
            pairs.begin(), pairs.end(), comp);

        result = var + " = phi " + type + ' ';
        for (auto iter = ordered.begin(); iter != ordered.end(); iter++) {
            if (iter != ordered.begin())
                result += ", ";
            std::string value = valueToString(iter->second);
            std::string bb_name =
                '%' + std::to_string(nir->getNumber(iter->first));
            result += "[ " + value + ", " + bb_name + " ]";
        }
    }

    void ToStringVisitor::visit(TerminatorReturn *t) {
        std::string value = valueToTypedString(t->getValue());

        result = "ret " + value;
    }

    void ToStringVisitor::visit(TerminatorBranch *t) {
        std::string bb = std::to_string(nir->getNumber(t->getSuccessor()));
        result = "br label " + bb;
    }

    void ToStringVisitor::visit(TerminatorCondBranch *t) {
        std::string cond = valueToTypedString(t->getCond());
        std::string bb_true =
            std::to_string(nir->getNumber(t->getTSuccessor()));
        std::string bb_false =
            std::to_string(nir->getNumber(t->getFSuccessor()));
        result = "br " + cond + ", label " + bb_true + ", label " + bb_false;
    }
} // namespace middleend::mir
