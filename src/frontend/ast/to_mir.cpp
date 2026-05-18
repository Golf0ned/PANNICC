#include "frontend/ast/to_mir.h"

#include <utility>

namespace mir = middleend::mir;

namespace frontend {

mir::Program lower(ast::Program &ast) {
    std::list<std::unique_ptr<mir::Function>> functions;
    mir::LiteralMap lm;

    ToMIRVisitor tmv(ast.getSymbolTable(), lm);
    for (auto &f : ast.getFunctions()) {
        f->accept(&tmv);
        functions.push_back(tmv.getResult()); // might need to move?
    }

    tmv.resolveFunctions();

    return mir::Program(std::move(functions), std::move(lm));
}

ToMIRVisitor::ToMIRVisitor(SymbolTable &st, mir::LiteralMap &lm)
    : st(st), lm(lm), cur_scope(0), cur_bb_id(1) {};

std::unique_ptr<mir::Function> ToMIRVisitor::getResult() {
    return std::move(res);
}

mir::InstructionAlloca *ToMIRVisitor::makeAlloca(mir::Type type) {
    auto alloca = std::make_unique<mir::InstructionAlloca>(type);
    auto *alloca_ptr = alloca.get();
    allocas.push_back(std::move(alloca));
    return alloca_ptr;
}

mir::Value *ToMIRVisitor::usePrevExpr() {
    auto *literal = dynamic_cast<mir::Literal *>(prev_expr);
    if (literal)
        return literal;

    auto *ptr = static_cast<mir::InstructionAlloca *>(prev_expr);
    auto type = ptr->getAllocType();

    auto load = std::make_unique<mir::InstructionLoad>(type, ptr);
    auto *load_ptr = load.get();
    instructions.push_back(std::move(load));
    return load_ptr;
}

mir::Literal *ToMIRVisitor::getLiteral(uint64_t value, mir::Type type) {
    auto &typed_map = lm[type];

    if (typed_map.contains(value))
        return typed_map.at(value).get();

    auto literal = std::make_unique<mir::Literal>(type, value);
    auto *literal_ptr = literal.get();
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

Type *ToMIRVisitor::getType(uint64_t id) {
    for (auto it = scope_binding_types.rbegin();
         it != scope_binding_types.rend(); it++) {
        auto bindings = *it;

        auto binding = bindings.find(id);
        if (binding != bindings.end())
            return binding->second;
    }
    std::unreachable();
}

std::vector<std::unique_ptr<mir::Value>>
ToMIRVisitor::makeParams(std::vector<ast::Parameter> &params) {
    std::vector<std::unique_ptr<mir::Value>> res;

    for (auto &[param_type, param_name] : params) {
        auto type = param_type->toMir();
        auto param = std::make_unique<mir::Value>(type);

        // TODO: replace with makeAlloca
        auto alloca = std::make_unique<mir::InstructionAlloca>(type);
        auto store =
            std::make_unique<mir::InstructionStore>(param.get(), alloca.get());

        scope_bindings.back()[param_name->getValue()] = alloca.get();
        scope_binding_types.back()[param_name->getValue()] = param_type.get();

        instructions.push_back(std::move(alloca));
        instructions.push_back(std::move(store));

        res.push_back(std::move(param));
    }

    return res;
}

void ToMIRVisitor::makeBB(std::unique_ptr<mir::Terminator> terminator) {
    auto bb = std::make_unique<mir::BasicBlock>(std::move(instructions),
                                                std::move(terminator));

    bb_ids[cur_bb_id++] = bb.get();

    basic_blocks.push_back(std::move(bb));
    instructions.clear();
}

void ToMIRVisitor::makeReturnBB(mir::Type return_type) {
    auto load = std::make_unique<mir::InstructionLoad>(return_type, ret_alloca);
    auto ret = std::make_unique<mir::TerminatorReturn>(load.get());

    std::list<std::unique_ptr<mir::Instruction>> instructions;
    instructions.push_back(std::move(load));

    auto bb = std::make_unique<mir::BasicBlock>(std::move(instructions),
                                                std::move(ret));

    bb_ids[0] = bb.get();
    basic_blocks.push_back(std::move(bb));
}

void ToMIRVisitor::resolveBBEdges() {
    for (auto &bb : basic_blocks) {
        auto *t = bb->getTerminator().get();

        auto br = dynamic_cast<mir::TerminatorBranch *>(t);
        if (br) {
            auto *succ = bb_ids.at(bb_edges[t][0]);
            br->setSuccessor(succ);
            bb->getSuccessors().addEdge(succ);
            succ->getPredecessors().addEdge(bb.get());
        }

        auto cond_br = dynamic_cast<mir::TerminatorCondBranch *>(t);
        if (cond_br) {
            auto *t_succ = bb_ids.at(bb_edges[t][0]);
            cond_br->setTSuccessor(t_succ);
            bb->getSuccessors().addEdge(t_succ);
            t_succ->getPredecessors().addEdge(bb.get());

            auto *f_succ = bb_ids.at(bb_edges[t][1]);
            cond_br->setFSuccessor(f_succ);
            bb->getSuccessors().addEdge(f_succ);
            f_succ->getPredecessors().addEdge(bb.get());
        }
    }
}

mir::BasicBlock *ToMIRVisitor::createEntryBlock() {
    auto &front_body = basic_blocks.front()->getInstructions();
    front_body.splice(front_body.begin(), allocas);

    return basic_blocks.front().get();
}

void ToMIRVisitor::resolveFunctions() {
    for (auto &[id, fun] : function_ids)
        for (auto *call : function_calls[id])
            call->setCallee(fun);
}

void ToMIRVisitor::visit(ast::FunctionDefinition *f) {
    scope_bindings.emplace_back();
    scope_binding_types.emplace_back();

    auto type = f->getType()->toMir();
    function_types[f->getName()->getValue()] = f->getType();
    auto name = f->getName()->toString(st);
    auto params = makeParams(f->getParameters());
    ret_alloca = makeAlloca(type);

    cur_scope++;
    scope_bindings.emplace_back();
    scope_binding_types.emplace_back();
    for (auto &i : f->getBody()->getInstructions())
        i->accept(this);

    if (name == "main" && (basic_blocks.size() == 0 || !instructions.empty())) {
        auto zero = getLiteral(0, mir::Type::I32);
        auto store = std::make_unique<mir::InstructionStore>(zero, ret_alloca);
        instructions.push_back(std::move(store));
    }

    auto br = std::make_unique<mir::TerminatorBranch>(nullptr);
    bb_edges[br.get()] = {0};
    makeBB(std::move(br));

    scope_bindings.pop_back();
    scope_binding_types.pop_back();
    cur_scope--;

    makeReturnBB(type);
    resolveBBEdges();
    auto *entry = createEntryBlock();

    res = std::make_unique<mir::FunctionDefinition>(
        type, name, std::move(params), std::move(basic_blocks), entry);
    // TODO: handle overloading
    function_ids[f->getName()->getValue()] = res.get();

    scope_bindings.pop_back();
    scope_binding_types.pop_back();
}

void ToMIRVisitor::visit(ast::FunctionPrototype *f) {
    scope_bindings.emplace_back();
    scope_binding_types.emplace_back();

    auto type = f->getType()->toMir();
    function_types[f->getName()->getValue()] = f->getType();
    auto name = f->getName()->toString(st);
    auto params = makeParams(f->getParameters());

    res = std::make_unique<mir::FunctionDeclaration>(type, name,
                                                     std::move(params));
    // TODO: handle overloading
    function_ids[f->getName()->getValue()] = res.get();

    scope_bindings.pop_back();
    scope_binding_types.pop_back();
}

void ToMIRVisitor::visit(ast::Instruction *i) {}

void ToMIRVisitor::visit(ast::Scope *s) {
    cur_scope++;
    scope_bindings.emplace_back();
    scope_binding_types.emplace_back();

    for (auto &i : s->getInstructions())
        i->accept(this);

    scope_bindings.pop_back();
    scope_binding_types.pop_back();
    cur_scope--;
}

void ToMIRVisitor::visit(ast::InstructionExpr *i) {
    i->getExpr()->accept(this);
}

void ToMIRVisitor::visit(ast::InstructionDeclaration *i) {
    auto type = i->getType()->toMir();
    scope_binding_types.back()[i->getVariable()->getValue()] = i->getType();

    auto *alloca = makeAlloca(type);
    scope_bindings.back()[i->getVariable()->getValue()] = alloca;

    if (i->getValue()) {
        i->getValue()->accept(this);
        auto *assign_val = usePrevExpr();
        auto store =
            std::make_unique<mir::InstructionStore>(assign_val, alloca);
        instructions.push_back(std::move(store));
    }
}

void ToMIRVisitor::visit(ast::InstructionReturn *i) {
    i->getValue()->accept(this);
    auto *ret_val = usePrevExpr();
    auto store = std::make_unique<mir::InstructionStore>(ret_val, ret_alloca);
    instructions.push_back(std::move(store));

    auto br = std::make_unique<mir::TerminatorBranch>(nullptr);
    bb_edges[br.get()] = {0};
    makeBB(std::move(br));
}

void ToMIRVisitor::visit(ast::InstructionIf *i) {
    i->getCond()->accept(this);
    auto *cond = usePrevExpr();

    auto cond_br =
        std::make_unique<mir::TerminatorCondBranch>(cond, nullptr, nullptr);
    auto *cond_br_ptr = cond_br.get();
    makeBB(std::move(cond_br));

    // Cond -> True
    bb_edges[cond_br_ptr] = {cur_bb_id};

    // True branch
    i->getTBranch()->accept(this);
    auto t_br = std::make_unique<mir::TerminatorBranch>(nullptr);
    auto *t_br_ptr = t_br.get();
    makeBB(std::move(t_br));

    // Cond -> [False or Cont]
    bb_edges[cond_br_ptr].push_back(cur_bb_id);

    // False branch
    if (i->getFBranch()) {
        i->getFBranch()->accept(this);
        auto f_br = std::make_unique<mir::TerminatorBranch>(nullptr);
        auto *f_br_ptr = f_br.get();
        makeBB(std::move(f_br));

        // False -> Cont
        bb_edges[f_br_ptr] = {cur_bb_id};
    }

    // True -> Cont
    bb_edges[t_br_ptr] = {cur_bb_id};
}

void ToMIRVisitor::visit(ast::InstructionWhile *i) {
    auto prev_br = std::make_unique<mir::TerminatorBranch>(nullptr);
    auto *prev_br_ptr = prev_br.get();
    makeBB(std::move(prev_br));
    // Prev -> Cond
    bb_edges[prev_br_ptr] = {cur_bb_id};

    auto cond_bb_id = cur_bb_id;
    i->getCond()->accept(this);
    auto *cond = usePrevExpr();
    auto cond_br =
        std::make_unique<mir::TerminatorCondBranch>(cond, nullptr, nullptr);
    auto *cond_br_ptr = cond_br.get();
    makeBB(std::move(cond_br));
    // Cond -> Body
    bb_edges[cond_br_ptr] = {cur_bb_id};

    i->getBody()->accept(this);
    auto body_br = std::make_unique<mir::TerminatorBranch>(nullptr);
    auto *body_br_ptr = body_br.get();
    makeBB(std::move(body_br));
    // Body -> Cond
    bb_edges[body_br_ptr] = {cond_bb_id};

    // Cond -> Cont
    bb_edges[cond_br_ptr].push_back(cur_bb_id);
}

void ToMIRVisitor::visit(ast::Expr *e) {}

void ToMIRVisitor::visit(ast::TerminalExpr *e) {
    if (e->getAtom()->isIdentifier()) {
        auto val = e->getAtom()->getValue();
        prev_expr = getUse(val);
        expr_types[e] = getType(val)->clone();
    } else {
        prev_expr = getLiteral(e->getAtom()->getValue(), mir::Type::I32);
        expr_types[e] = std::make_unique<Int>();
    }
}

void ToMIRVisitor::visit(ast::ParenExpr *e) {
    e->getBody()->accept(this);
    expr_types[e] = expr_types[e->getBody()]->clone();
}

void ToMIRVisitor::visit(ast::CallExpr *e) {
    auto callee_id = e->getCallee()->getValue();
    auto type = function_types[callee_id]->toMir();

    std::vector<mir::Value *> args;
    for (auto &arg : e->getArguments()) {
        arg->accept(this);
        args.push_back(usePrevExpr());
    }

    auto call =
        std::make_unique<mir::InstructionCall>(type, nullptr, std::move(args));
    function_calls[callee_id].push_back(call.get());

    auto *alloca = makeAlloca(type);
    auto store = std::make_unique<mir::InstructionStore>(call.get(), alloca);

    instructions.push_back(std::move(call));
    instructions.push_back(std::move(store));

    prev_expr = alloca;
    expr_types[e] = function_types[callee_id]->clone();
}

void ToMIRVisitor::visit(ast::UnaryOpExpr *e) {
    // TODO: please bleach this

    auto type = mir::Type::I32;
    e->getValue()->accept(this);
    auto *val = usePrevExpr();

    mir::Value *un_op_res, *literal;
    std::unique_ptr<mir::Instruction> bin_op;
    Type *ptr_type;
    switch (e->getOp()) {
    case UnaryOp::PLUS:
        // Do nothing
        un_op_res = val;
        expr_types[e] = std::make_unique<Int>();
        break;
    case UnaryOp::MINUS:
        // Subtract from zero
        literal = getLiteral(0, mir::Type::I32);
        bin_op = std::make_unique<mir::InstructionBinaryOp>(
            type, mir::BinaryOp::SUB, literal, val);

        un_op_res = static_cast<mir::InstructionBinaryOp *>(bin_op.get());
        instructions.push_back(std::move(bin_op));
        expr_types[e] = std::make_unique<Int>();
        break;
    case UnaryOp::NOT:
        // XOR with -1
        literal = getLiteral(-1, mir::Type::I32);
        bin_op = std::make_unique<mir::InstructionBinaryOp>(
            type, mir::BinaryOp::XOR, val, literal);

        un_op_res = static_cast<mir::InstructionBinaryOp *>(bin_op.get());
        instructions.push_back(std::move(bin_op));
        expr_types[e] = std::make_unique<Int>();
        break;
    case UnaryOp::DEREF:
        // Load from val
        ptr_type =
            dynamic_cast<Ptr *>(expr_types[e->getValue()].get())->getBase();
        type = ptr_type->toMir();
        bin_op = std::make_unique<mir::InstructionLoad>(type, val);

        un_op_res = static_cast<mir::InstructionLoad *>(bin_op.get());
        instructions.push_back(std::move(bin_op));
        expr_types[e] = ptr_type->clone();
        break;
    case UnaryOp::ADDRESS:
        type = mir::Type::PTR;

        un_op_res = prev_expr;
        expr_types[e] =
            std::make_unique<Ptr>(expr_types[e->getValue()]->clone());
        break;
    }

    auto *res_ptr = makeAlloca(type);
    prev_expr = res_ptr;

    auto store = std::make_unique<mir::InstructionStore>(un_op_res, res_ptr);
    instructions.push_back(std::move(store));
}

void ToMIRVisitor::visit(ast::BinaryOpExpr *e) {
    e->getRight()->accept(this);
    auto *right = usePrevExpr();
    e->getLeft()->accept(this);
    // TODO: figure type inference out
    auto type = expr_types[e->getLeft()]->toMir();

    mir::Value *bin_op_res;
    Type *ptr_type;
    if (!isAssignment(e->getOp())) {
        // Standard binary op
        auto *left = usePrevExpr();
        auto bin_op = std::make_unique<mir::InstructionBinaryOp>(
            type, toMir(e->getOp()), left, right);

        bin_op_res = static_cast<mir::InstructionBinaryOp *>(bin_op.get());
        instructions.push_back(std::move(bin_op));
    } else if (e->getOp() != BinaryOp::ASSIGN) {
        // Op assign
        auto load = std::make_unique<mir::InstructionLoad>(type, prev_expr);
        auto *left = load.get();
        instructions.push_back(std::move(load));

        auto bin_op = std::make_unique<mir::InstructionBinaryOp>(
            type, toMir(e->getOp()), left, right);

        bin_op_res = static_cast<mir::InstructionBinaryOp *>(bin_op.get());
        instructions.push_back(std::move(bin_op));
    } else {
        // Just assign
        bin_op_res = right;
    }

    auto *res_ptr =
        e->getOp() == BinaryOp::ASSIGN ? prev_expr : makeAlloca(type);
    prev_expr = res_ptr;

    auto store = std::make_unique<mir::InstructionStore>(bin_op_res, res_ptr);
    instructions.push_back(std::move(store));
    expr_types[e] = expr_types[e->getLeft()]->clone();
}

} // namespace frontend
