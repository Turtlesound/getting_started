#ifndef BYTECODE_INTERPRETER_H
#define BYTECODE_INTERPRETER_H

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include <iostream>

// Bytecode instruction structure
struct BytecodeInstruction {
    std::string opcode;
    std::vector<std::string> operands;
    
    BytecodeInstruction(const std::string& op) : opcode(op) {}
    BytecodeInstruction(const std::string& op, const std::string& arg1) : opcode(op) {
        operands.push_back(arg1);
    }
    BytecodeInstruction(const std::string& op, const std::string& arg1, const std::string& arg2) : opcode(op) {
        operands.push_back(arg1);
        operands.push_back(arg2);
    }
};

// Activation record for method calls
struct ActivationRecord {
    std::string methodName;
    std::map<int, int> localVariables;  // index -> value
    int returnAddress;
    int previousPC;
    
    ActivationRecord() : returnAddress(0), previousPC(0) {}
    ActivationRecord(const std::string& method, int retAddr, int prevPC) 
        : methodName(method), returnAddress(retAddr), previousPC(prevPC) {}
};

// Method information
struct MethodInfo {
    std::string className;
    std::string methodName;
    std::string signature;
    int startAddress;
    std::vector<BytecodeInstruction> instructions;
};

class BytecodeInterpreter {
public:
    bool verbose = false;  // For debugging output
    static const int MAX_RECURSION_DEPTH = 1000;  // Stack overflow protection
    
    BytecodeInterpreter();
    ~BytecodeInterpreter();
    
    // Load bytecode from file
    bool loadBytecode(const std::string& filename);
    
    // Execute the loaded bytecode
    void execute();
    
    // Get the current data stack (for debugging)
    std::stack<int> getDataStack() const { return dataStack; }
    
private:
    // Two stacks for stack-machine execution
    std::stack<int> dataStack;                    // Data values stack
    std::stack<ActivationRecord> activationStack; // Method call stack
    
    // Program storage
    std::vector<BytecodeInstruction> instructions;
    std::map<std::string, MethodInfo> methods;    // method name -> method info
    std::map<std::string, int> labels;            // label -> instruction address
    
    // Execution state
    int programCounter;
    bool running;
    std::map<int, int> globalLocalVars;  // Global local variables for main method
    
    // Helper methods for parsing
    void parseLine(const std::string& line);
    bool isLabel(const std::string& line);
    std::string extractLabel(const std::string& line);
    BytecodeInstruction parseInstruction(const std::string& line);
    std::vector<std::string> tokenize(const std::string& str);
    std::string trim(const std::string& str);
    
    // Instruction execution methods
    void executeInstruction(const BytecodeInstruction& instr);
    
    // Stack operations
    void executeILoad(int index);
    void executeIConst(int value);
    void executeIStore(int index);
    
    // Arithmetic operations
    void executeIAdd();
    void executeISub();
    void executeIMul();
    void executeIDiv();
    
    // Comparison operations
    void executeILt();
    void executeIGt();
    void executeIEq();
    
    // Logical operations
    void executeIAnd();
    void executeIOr();
    void executeINot();
    
    // Control flow operations
    void executeGoto(const std::string& label);
    void executeIfFalse(const std::string& label);
    
    // Method operations
    void executeInvokeVirtual(const std::string& methodName);
    void executeIReturn();
    
    // I/O operations
    void executePrint();
    void executeStop();
    
    // Helper methods
    int getCurrentLocalVar(int index);
    void setCurrentLocalVar(int index, int value);
    void printDebugInfo(const BytecodeInstruction& instr);
    bool isNumeric(const std::string& str);
    int stringToInt(const std::string& str);
};

#endif // BYTECODE_INTERPRETER_H
