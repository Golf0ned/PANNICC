#include "frontend/parser/instruction.h"

using namespace frontend;

std::vector<std::unique_ptr<ast::Scope>> frontend::active_scopes;

std::unique_ptr<ast::Instruction> popInstruction() {
    auto i = std::move(active_scopes.back()->getInstructions().back());
    active_scopes.back()->getInstructions().pop_back();
    return i;
}

std::unique_ptr<ast::Scope> popScope() {
    auto scope = std::move(active_scopes.back());
    active_scopes.pop_back();
    return scope;
}
