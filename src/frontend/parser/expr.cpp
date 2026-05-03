#include "frontend/parser/expr.h"

using namespace frontend;

std::vector<Token> frontend::parsed_tokens;
std::vector<std::unique_ptr<ast::Expr>> frontend::parsed_exprs;
std::vector<std::unique_ptr<ast::Expr>> frontend::active_args;

std::unique_ptr<Atom> popAtom() {
    auto [token_val, token_type] = parsed_tokens.back();
    parsed_tokens.pop_back();
    if (token_type == TokenKind::NUMBER) {
        long long parsed_val = std::stoll(token_val);
        int32_t truncated = static_cast<int32_t>(parsed_val);
        uint64_t val = static_cast<uint64_t>(static_cast<int64_t>(truncated));
        return std::make_unique<AtomLiteral>(val);
    } else
        return std::make_unique<AtomIdentifier>(st.addSymbol(token_val));
}

std::unique_ptr<AtomIdentifier> popIdentifier() {
    auto [token_val, token_type] = parsed_tokens.back();
    parsed_tokens.pop_back();
    return std::make_unique<AtomIdentifier>(st.addSymbol(token_val));
}

std::unique_ptr<ast::Expr> popExpr() {
    auto expr = std::move(parsed_exprs.back());
    parsed_exprs.pop_back();
    return expr;
}
