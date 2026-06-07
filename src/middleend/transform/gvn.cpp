#include "middleend/transform/gvn.h"

namespace middleend {
void GVN::run(mir::Program &p) {
    for (auto &f : p.getFunctions()) {
        auto definition = dynamic_cast<mir::FunctionDefinition *>(f.get());
        if (!definition)
            continue;
    }
}

} // namespace middleend
