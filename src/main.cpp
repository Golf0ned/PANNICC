#include <iostream>

#include "frontend/parser.h"
#include "frontend/ast.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];

    frontend::AST ast = frontend::parse(input_file);
    std::cout << "Parsed AST: " << ast.toString() << std::endl;

    return 0;
}
