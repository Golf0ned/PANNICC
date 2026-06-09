#include "middleend/mir/value.h"
#include "middleend/mir/type.h"

namespace middleend::mir {

Value::Value(Type type) : type(type) {}

Type Value::getType() { return type; }

std::unordered_map<Instruction *, uint64_t> &Value::getUses() { return uses; }

Literal::Literal(Type type, int64_t value) : Value(type), value(value) {}

int64_t Literal::getValue() { return value; }

std::unordered_map<Type, std::unordered_map<uint64_t, std::unique_ptr<Literal>>>
    LiteralMap::literals;

Literal *LiteralMap::getLiteral(Type type, uint64_t value) {
    auto &typed_map = literals[type];
    if (typed_map.contains(value))
        return typed_map.at(value).get();

    auto literal = std::make_unique<Literal>(type, value);
    auto *literal_ptr = literal.get();

    typed_map[value] = std::move(literal);
    return literal_ptr;
}

} // namespace middleend::mir
