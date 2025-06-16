#ifndef BYTECODE_INTERPRETER_H
#define BYTECODE_INTERPRETER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

class BytecodeInterpreter {
public:
    // Data structures for bytecode representation
    struct Method {
        std::string name;
        std::string signature;
        std::vector<std::string> instructions;
        std::map<std::string, int> labels;
        int maxLocals;
        
        Method() : maxLocals(0) {}
    };
    
    struct Class {
        std::string name;
        std::string superclass;
        std::map<std::string, std::shared_ptr<Method>> methods;
    };
    
    // Runtime data structures
    struct Frame {
        std::string methodName;
        std::string className;
        std::vector<int> locals;
        int returnPC;
    };
    
    struct Array {
        std::vector<int> elements;
    };
    
    struct Object {
        std::string className;
        std::map<std::string, int> fields;
    };
    
    // Maps and storage
    std::map<std::string, std::shared_ptr<Class>> classes;
    std::string mainClassName;
    
    std::vector<int> dataStack;
    std::vector<Frame> activationStack;
    
    std::vector<std::shared_ptr<Array>> arrays;
    std::vector<std::shared_ptr<Object>> objects;
    
    // Helper methods
    void push(int value);
    int pop();
    int peek();
    int resolveLabel(const std::string& label, const std::shared_ptr<Method>& method);
    int allocateArray(int size);
    int allocateObject(const std::string& className);
    void printStackState();
    
    BytecodeInterpreter();
    
    bool loadBytecode(const std::string& filename);
    void execute();
    void executeMethod(const std::string& className, const std::string& methodName);
    void executeMethod(const std::string& className, const std::string& methodName, int objRef, const std::vector<int>& params); // Modified line
    
    void executeInstruction(const std::string& instr, int& pc);
    
    // Verbose mode flag
    bool verbose = false;
};

#endif // BYTECODE_INTERPRETER_H