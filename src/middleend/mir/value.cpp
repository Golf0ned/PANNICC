#include "middleend/mir/value.h"
#include "middleend/mir/type.h"

namespace middleend::mir {
    Value::Value(Type type) : type(type) {}

    Type Value::getType() { return type; }

    Literal::Literal(Type type, int64_t value) : Value(type), value(value) {}

    int64_t Literal::getValue() { return value; }
} // namespace middleend::mir
