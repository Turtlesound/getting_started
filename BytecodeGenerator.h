#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include "IR.h"
#include <string>
#include <unordered_set>
#include <map>
#include <vector>
#include <fstream>

class BytecodeGenerator {
public:
    BytecodeGenerator();
    
    // Generate bytecode for a CFG
    void generateBytecode(const CFG& cfg, const std::string& outputFileName);

private:
    std::unordered_set<BasicBlock*> visitedBlocks;
    std::map<std::string, int> localVarIndices;
    std::map<std::string, int> methodLabels;
    std::map<std::string, std::string> methodSignatures;  // Maps method names to their signatures
    int localVarCounter;
    int pendingParamCount;  // Track parameters for method calls
    int labelCounter;       // For generating unique labels
    std::ofstream outputFile;
    
    // Helper method to get or assign a local variable index
    int getVarIndex(const std::string& varName);
    
    // Helper method to generate labels for blocks
    std::string getBlockLabel(BasicBlock* block);
    
    // Helper method to generate unique labels
    std::string generateUniqueLabel();
    
    // Process a single IR instruction and generate bytecode
    void generateInstructionBytecode(const Instruction& instr);
    
    // Generate bytecode for a block and its successors
    void generateBlockBytecode(BasicBlock* block);
    int analyzeMethodParameters(BasicBlock* entryBlock, const std::string& methodName);
};

#endif // BYTECODE_GENERATOR_H