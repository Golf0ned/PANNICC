#include "frontend/ast/to_mir.h"

#include <utility>

using namespace frontend;

namespace mir = middleend::mir;

mir::Program lower(ast::Program &ast) {
    std::list<std::unique_ptr<mir::Function>> functions;
    mir::LiteralMap lm;

    ToMIRVisitor tmv(ast.getSymbolTable(), lm);
    for (auto &f : ast.getFunctions()) {
        f->accept(&tmv);
        functions.push_back(tmv.getResult()); // might need to move?
    }

    return mir::Program(std::move(functions), std::move(lm));
}

ToMIRVisitor::ToMIRVisitor(SymbolTable &st, mir::LiteralMap &lm)
    : st(st), lm(lm), cur_scope(0) {};

std::unique_ptr<mir::Function> ToMIRVisitor::getResult() {
    return std::move(res);
}

mir::InstructionAlloca *ToMIRVisitor::makeAlloca(mir::Type type) {
    auto alloca = std::make_unique<mir::InstructionAlloca>(type);
    auto alloca_ptr = alloca.get();
    allocas.push_back(std::move(alloca));
    return alloca_ptr;
}

mir::Literal *ToMIRVisitor::getLiteral(uint64_t value, mir::Type type) {
    auto &typed_map = lm[type];

    if (typed_map.contains(value))
        return typed_map.at(value).get();

    auto literal = std::make_unique<middleend::mir::Literal>(type, value);
    auto literal_ptr = literal.get();
    typed_map[value] = std::move(literal);
    return literal_ptr;
}

mir::Value *ToMIRVisitor::getUse(uint64_t id) {
    for (auto it = scope_bindings.rbegin(); it != scope_bindings.rend(); it++) {
        auto bindings = *it;

        auto binding = bindings.find(id);
        if (binding != bindings.end())
            return binding->second;
    }
    std::unreachable();
}

std::vector<std::unique_ptr<mir::Value>>
makeParams(std::vector<ast::Parameter> &params) {
    std::vector<std::unique_ptr<mir::Value>> res;

    for (auto &[param_type, param_name] : params) {
        // TODO: make variable (with scope?)
        // TODO: make alloca + store, map to param name
        // TODO: map variable type?
        // TODO: add variable to res
    }

    return res;
}

void ToMIRVisitor::resolveBBEdges() {
    // TODO
}

middleend::mir::BasicBlock *ToMIRVisitor::createEntryBlock() {
    auto &front_body = basic_blocks.front()->getInstructions();
    front_body.splice(front_body.begin(), allocas);

    return basic_blocks.front().get();
}

void ToMIRVisitor::resolveFunctions() {
    // TODO
}

void ToMIRVisitor::visit(ast::FunctionDefinition *f) {
    auto type = f->getType()->toMir();
    auto name = f->getName()->toString(st);
    auto params = makeParams(f->getParameters());

    ret_alloca = makeAlloca(type);

    // TODO: map function type?
    // TODO: map function in preparation for resolveFunctions

    // Lower body
    visit(f->getBody());
    resolveBBEdges();
    auto entry = createEntryBlock();

    res = std::make_unique<mir::FunctionDefinition>(
        type, name, std::move(params), std::move(basic_blocks), entry);
}

void ToMIRVisitor::visit(ast::FunctionPrototype *f) {
    auto type = f->getType()->toMir();
    auto name = f->getName()->toString(st);
    auto params = makeParams(f->getParameters());

    // TODO: map function type?
    // TODO: map function in preparation for resolveFunctions

    res = std::make_unique<mir::FunctionDeclaration>(type, name,
                                                     std::move(params));
}

void ToMIRVisitor::visit(ast::Instruction *i) {}

void ToMIRVisitor::visit(ast::Scope *s) {
    cur_scope++;
    scope_bindings.emplace_back();

    for (auto &i : s->getInstructions())
        i->accept(this);

    scope_bindings.pop_back();
    cur_scope--;
}

void ToMIRVisitor::visit(ast::InstructionExpr *i) {
    i->getExpr()->accept(this);
}

void ToMIRVisitor::visit(ast::InstructionDeclaration *i) {
    auto type = i->getType()->toMir();
    auto alloca = makeAlloca(type);

    // TODO: map value and type?

    if (i->getValue()) {
        i->getValue()->accept(this);
        auto assign_val = prev_expr;
        auto store =
            std::make_unique<mir::InstructionStore>(assign_val, alloca);
        instructions.push_back(std::move(store));
    }
}

void ToMIRVisitor::visit(ast::InstructionReturn *i) {
    i->getValue()->accept(this);
    auto ret_val = prev_expr;
    auto store = std::make_unique<mir::InstructionStore>(ret_val, ret_alloca);
    instructions.push_back(std::move(store));

    // TODO: set branch up for connecting BBs
    auto branch = std::make_unique<mir::TerminatorBranch>(nullptr);
    auto bb = std::make_unique<mir::BasicBlock>(std::move(instructions),
                                                std::move(branch));
    basic_blocks.push_back(std::move(bb));
    instructions.clear();

    // TODO: map branch
}

void ToMIRVisitor::visit(ast::InstructionIf *i) {}

void ToMIRVisitor::visit(ast::InstructionWhile *i) {}

void ToMIRVisitor::visit(ast::Expr *e) {}

void ToMIRVisitor::visit(ast::TerminalExpr *e) {
    prev_expr = e->getAtom()->isIdentifier()
                    ? getUse(e->getAtom()->getValue())
                    : getLiteral(e->getAtom()->getValue(), mir::Type::I32);
}

void ToMIRVisitor::visit(ast::ParenExpr *e) { e->getBody()->accept(this); }

void ToMIRVisitor::visit(ast::CallExpr *e) {}

void ToMIRVisitor::visit(ast::UnaryOpExpr *e) {
    // TODO: review it all

    // TODO: figure out type
    auto type = mir::Type::I32;

    e->getValue()->accept(this);
    auto val = prev_expr;
    auto res_ptr = makeAlloca(type);

    middleend::mir::Value *un_op_res, *literal;
    std::unique_ptr<middleend::mir::Instruction> bin_op;
    switch (e->getOp()) {
    case UnaryOp::PLUS:
        // Do nothing
        // TODO: cast to int?
        un_op_res = val;
        break;
    case UnaryOp::MINUS:
        // Subtract from zero
        literal = getLiteral(0, mir::Type::I32);
        bin_op = std::make_unique<mir::InstructionBinaryOp>(
            type, mir::BinaryOp::SUB, literal, val);
        un_op_res = static_cast<mir::Value *>(
            static_cast<mir::InstructionBinaryOp *>(bin_op.get()));
        instructions.push_back(std::move(bin_op));
        break;
    case UnaryOp::NOT:
    case UnaryOp::ADDRESS:
    case UnaryOp::DEREF:
    }
}

void ToMIRVisitor::visit(ast::BinaryOpExpr *e) {}
