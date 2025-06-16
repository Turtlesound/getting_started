#include "BytecodeInterpreter.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <bytecode_file> [-v]" << std::endl;
        return 1;
    }
    
    BytecodeInterpreter interpreter;
    
    // Check for verbose flag
    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--verbose") {
            verbose = true;
            interpreter.verbose = true;
        }
    }
    
    if (!interpreter.loadBytecode(argv[1])) {
        std::cerr << "Failed to load bytecode file: " << argv[1] << std::endl;
        return 1;
    }
    
    std::cout << "Executing bytecode..." << std::endl;
    interpreter.execute();
    std::cout << "Execution completed." << std::endl;
    
    return 0;
}