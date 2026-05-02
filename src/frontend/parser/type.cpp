#include "frontend/parser/type.h"

using namespace frontend;

std::vector<std::unique_ptr<Type>> frontend::parsed_types;

std::unique_ptr<Type> popType() {
    auto type = std::move(parsed_types.back());
    parsed_types.pop_back();
    return type;
}
