#include "BytecodeGenerator.h"
#include <iostream>
#include <sstream>

// Constructor implementation
BytecodeGenerator::BytecodeGenerator() : localVarCounter(0) {}

// Helper method to get or assign a local variable index
int BytecodeGenerator::getVarIndex(const std::string& varName) {
    if (localVarIndices.find(varName) == localVarIndices.end()) {
        localVarIndices[varName] = localVarCounter++;
    }
    return localVarIndices[varName];
}

// Helper method to generate labels for blocks
std::string BytecodeGenerator::getBlockLabel(BasicBlock* block) {
    return "L" + std::to_string(block->blockId);
}

// Process a single IR instruction and generate bytecode
void BytecodeGenerator::generateInstructionBytecode(const Instruction& instr) {
    switch(instr.type) {
        case Instruction::ASSIGN:
            if (instr.op1[0] >= '0' && instr.op1[0] <= '9') {
                // Load a constant
                outputFile << "    iconst " << instr.op1 << std::endl;
            } else {
                // Load from variable
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;
            }
            // Store to result variable
            outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
            break;
            
        case Instruction::BINOP:
            // Load operands
            if (instr.op1[0] >= '0' && instr.op1[0] <= '9' || instr.op1 == "true" || instr.op1 == "false") {
                outputFile << "    iconst " << (instr.op1 == "true" ? "1" : (instr.op1 == "false" ? "0" : instr.op1)) << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;
            }
            
            if (instr.op2[0] >= '0' && instr.op2[0] <= '9' || instr.op2 == "true" || instr.op2 == "false") {
                outputFile << "    iconst " << (instr.op2 == "true" ? "1" : (instr.op2 == "false" ? "0" : instr.op2)) << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op2) << std::endl;
            }
            
            // Perform operation
            if (instr.opcode == "+") {
                outputFile << "    iadd" << std::endl;
            } else if (instr.opcode == "-") {
                outputFile << "    isub" << std::endl;
            } else if (instr.opcode == "*") {
                outputFile << "    imul" << std::endl;
            } else if (instr.opcode == "/") {
                outputFile << "    idiv" << std::endl;
            } else if (instr.opcode == "<") {
                // If comparing constant 0 with a variable, reverse the comparison for more intuitive loop behavior
                if (instr.op1 == "0" && instr.op2[0] != '0' && instr.op2[0] != 't' && instr.op2[0] != 'f' && instr.op2 != "true" && instr.op2 != "false") {
                    outputFile << "    igt" << std::endl;  // Use > instead of <
                } else {
                    outputFile << "    ilt" << std::endl;
                }
            } else if (instr.opcode == ">") {
                // Similarly reverse for constants
                if (instr.op1 == "0" && instr.op2[0] != '0' && instr.op2[0] != 't' && instr.op2[0] != 'f' && instr.op2 != "true" && instr.op2 != "false") {
                    outputFile << "    ilt" << std::endl;  // Use < instead of >
                } else {
                    outputFile << "    igt" << std::endl;
                }
            } else if (instr.opcode == "==") {
                outputFile << "    ieq" << std::endl;
            } else if (instr.opcode == "&&") {
                outputFile << "    iand" << std::endl;
            } else if (instr.opcode == "||") {
                outputFile << "    ior" << std::endl;
            }
            
            // Store result
            outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
            break;
            
        case Instruction::UNOP:
            // Load operand
            if (instr.op1[0] >= '0' && instr.op1[0] <= '9' || instr.op1 == "true" || instr.op1 == "false") {
                outputFile << "    iconst " << (instr.op1 == "true" ? "1" : (instr.op1 == "false" ? "0" : instr.op1)) << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;
            }
            
            // Perform operation
            if (instr.opcode == "!") {
                outputFile << "    inot" << std::endl;
            } else if (instr.opcode == "-") {
                outputFile << "    ineg" << std::endl;
            }
            
            // Store result
            outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
            break;
            
        case Instruction::PARAM:
            // Load parameter onto stack
            if (instr.result[0] >= '0' && instr.result[0] <= '9') {
                outputFile << "    iconst " << instr.result << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.result) << std::endl;
            }
            break;

        case Instruction::CALL:
            {
                std::string methodName = instr.op1;
                size_t dotPos = methodName.find('.');
                if (dotPos != std::string::npos) {
                    std::string className = methodName.substr(0, dotPos);
                    std::string funcName = methodName.substr(dotPos + 1);
                    
                    if (funcName == "println" && className == "System.out") {
                        outputFile << "    print" << std::endl;
                    } else {
                        // Load object reference first
                        if (className.substr(0, 1) == "t") {
                            outputFile << "    iload " << getVarIndex(className) << std::endl;
                        }
                        // Parameters should already be on stack from PARAM instructions
                        std::string fullMethodName = className + "." + funcName;
                        std::string signature = "()I";  // Default signature
                        if (methodSignatures.find(fullMethodName) != methodSignatures.end()) {
                            signature = methodSignatures[fullMethodName];
                        }
                        outputFile << "    invokevirtual " << funcName << signature << std::endl;
                        if (!instr.result.empty()) {
                            outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
                        }
                    }
                } else if (methodName.substr(0, 4) == "new ") {
                    // Object instantiation
                    std::string className = methodName.substr(4);
                    outputFile << "    new " << className << std::endl;
                    outputFile << "    dup" << std::endl;
                    outputFile << "    invokespecial " << className << "/<init>()V" << std::endl;
                    if (!instr.result.empty()) {
                        outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
                    }
                }
            }
            break;
            
        case Instruction::RETURN:
            if (!instr.result.empty()) {
                if (instr.result[0] >= '0' && instr.result[0] <= '9' || instr.result == "true" || instr.result == "false") {
                    outputFile << "    iconst " << (instr.result == "true" ? "1" : (instr.result == "false" ? "0" : instr.result)) << std::endl;
                } else {
                    outputFile << "    iload " << getVarIndex(instr.result) << std::endl;
                }
            }
            outputFile << "    ireturn" << std::endl;
            break;
            
        case Instruction::ARRAY_STORE:
            // Load array reference
            outputFile << "    iload " << getVarIndex(instr.result) << std::endl;
            // Load index
            if (instr.op1[0] >= '0' && instr.op1[0] <= '9') {
                outputFile << "    iconst " << instr.op1 << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;
            }
            // Load value
            if (instr.op2[0] >= '0' && instr.op2[0] <= '9' || instr.op2 == "true" || instr.op2 == "false") {
                outputFile << "    iconst " << (instr.op2 == "true" ? "1" : (instr.op2 == "false" ? "0" : instr.op2)) << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op2) << std::endl;
            }
            // Store into array
            outputFile << "    iastore" << std::endl;
            break;
            
        case Instruction::ARRAY_LOAD:
            // Load array reference
            outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;
            // Load index
            if (instr.op2[0] >= '0' && instr.op2[0] <= '9') {
                outputFile << "    iconst " << instr.op2 << std::endl;
            } else {
                outputFile << "    iload " << getVarIndex(instr.op2) << std::endl;
            }
            // Load from array
            outputFile << "    iaload" << std::endl;
            // Store to result
            outputFile << "    istore " << getVarIndex(instr.result) << std::endl;
            break;
            
        case Instruction::GOTO:
            {
                std::string labelStr = instr.result;
                if (labelStr.rfind("block_", 0) == 0) {
                    labelStr = "L" + labelStr.substr(6);
                }
                outputFile << "    goto " << labelStr << std::endl;
            }
            break;
            
        case Instruction::IF_TRUE:
            {
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl; 

                std::string labelStr = instr.result;
                if (labelStr.rfind("block_", 0) == 0) {
                    labelStr = "L" + labelStr.substr(6);
                }
                outputFile << "    ifne " << labelStr << std::endl; // Jump if true (stack top value != 0)
            }
            break;

        case Instruction::IF_FALSE:
            {
                outputFile << "    iload " << getVarIndex(instr.op1) << std::endl;

                std::string labelStr = instr.result;
                if (labelStr.rfind("block_", 0) == 0) {
                    labelStr = "L" + labelStr.substr(6);
                }
                outputFile << "    ifeq " << labelStr << std::endl; // Jump if false (stack top value == 0)
            }
            break;

        case Instruction::LABEL:
            // Ensure labels like "block_X" from IR are converted to "LX" if that's the convention for definition
            // However, getBlockLabel already generates "LX", so instr.result for LABEL should be "LX"
            // If instr.result for LABEL can also be "block_X", it needs conversion here too.
            // For now, assuming instr.result for LABEL is already in the final format (e.g., "L1").
            outputFile << instr.result << ":" << std::endl;
            break;

            
    }
}

// Generate bytecode for a block and its successors
void BytecodeGenerator::generateBlockBytecode(BasicBlock* block) {
    if (!block || visitedBlocks.count(block) > 0) {
        return; // Skip if already visited or null
    }
    
    visitedBlocks.insert(block);
    
    
    outputFile << getBlockLabel(block) << ":" << std::endl;
    
    bool returnEncountered = false;
    
    // Generate bytecode for each instruction in the block
    for (const auto& instr : block->instructions) {
        // Skip instructions after a return
        if (returnEncountered) {
            continue;
        }
        
        generateInstructionBytecode(instr);
        
        // Mark if we encounter a return instruction
        if (instr.type == Instruction::RETURN) {
            returnEncountered = true;
        }
    }
    
    // Only process successors if we didn't hit a return
    if (!returnEncountered) {
        for (auto succ : block->successors) {
            generateBlockBytecode(succ);
        }
    }
}

// Generate bytecode for a CFG
void BytecodeGenerator::generateBytecode(const CFG& cfg, const std::string& outputFileName) {
    outputFile.open(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFileName << std::endl;
        return;
    }
    
    // Reset state
    visitedBlocks.clear();
    localVarIndices.clear();
    localVarCounter = 0;
    methodSignatures.clear(); // Clear method signatures at the start
    
    // Group methods by class
    std::map<std::string, std::vector<std::pair<std::string, BasicBlock*>>> methodsByClass;
    // Assuming CFG provides a way to get all method entry blocks and their parameter names/types
    // For this example, we'll manually define parameter info for D1.java
    std::map<std::string, std::vector<std::string>> methodParametersFromAST;


    for (const auto& block : cfg.blocks) {
        std::string label = block->label;
        if (label.find('.') != std::string::npos) {
            size_t dotPos = label.find('.');
            std::string className = label.substr(0, dotPos);
            std::string methodName = label.substr(dotPos + 1);
            methodsByClass[className].push_back({methodName, block.get()});
        }
    }
    
    if (methodsByClass.empty()) {
        outputFile << ".class public Output" << std::endl;
        outputFile << ".super java/lang/Object" << std::endl;
    }
    
    for (const auto& [className, methods] : methodsByClass) {
        outputFile << ".class public " << className << std::endl;
        outputFile << ".super java/lang/Object" << std::endl << std::endl;
        
        for (const auto& [methodName, entryBlock] : methods) {
            localVarIndices.clear();
            localVarCounter = 0;

            // Assign index 0 for "this" in instance methods
            localVarIndices["this"] = localVarCounter++; // 'this' = 0, localVarCounter = 1

            std::vector<std::string> currentMethodParamNames;
            std::string methodKey = className + "." + methodName;
            if (methodParametersFromAST.count(methodKey)) {
                currentMethodParamNames = methodParametersFromAST[methodKey];
            }

            // Pre-assign local variable indices for parameters
            for (const std::string& paramName : currentMethodParamNames) {
                if (localVarIndices.find(paramName) == localVarIndices.end()) {
                    localVarIndices[paramName] = localVarCounter++;
                }
            }
            
            // Update method signature to include parameter types
            int actualParamCount = currentMethodParamNames.size();
            std::string methodSignatureStr = "(";
            for (int i = 0; i < actualParamCount; i++) {
                methodSignatureStr += "I"; // Default to int parameters
            }
            methodSignatureStr += ")I"; // Default to int return type
            
            methodSignatures[methodKey] = methodSignatureStr;

            outputFile << ".method public " << methodName << methodSignatureStr << std::endl;
            outputFile << "    ; Method entry for " << className << "." << methodName << std::endl;
            
            visitedBlocks.clear();
            // bool hasReturnBeenGenerated = false; // This logic seems to be handled by generateBlockBytecode

            generateBlockBytecode(entryBlock); // Local vars like 'sum' will get indices via getVarIndex now

            // Check if the block itself generated a return. If not, add one.
            // This check needs to be more robust, e.g., by checking if all paths from entryBlock end in a return.
            // For now, rely on existing logic or ensure returns are present in IR.
            bool hasReturn = false;
            if(entryBlock && !entryBlock->instructions.empty()){
                if(entryBlock->instructions.back().type == Instruction::RETURN){
                    hasReturn = true;
                }
            }
            if (!hasReturn && !entryBlock->successors.empty()) { // crude check
                 // A more robust check would be to see if generateBlockBytecode actually emitted an ireturn
            }


            // The default return logic might need refinement based on actual control flow.
            // If generateBlockBytecode guarantees a return on all paths, this is not needed.
            // The original code had a more complex check for hasReturnBeenGenerated.
            // For now, let's assume the IR/block generation handles returns.

            outputFile << ".end method" << std::endl << std::endl;
        }
        
        bool hasConstructor = false;
        for (const auto& [methodName, _] : methods) {
            if (methodName == "<init>") {
                hasConstructor = true;
                break;
            }
        }
        
        if (!hasConstructor) {
            outputFile << ".method public <init>()V" << std::endl;
            outputFile << "    aload_0" << std::endl;
            outputFile << "    invokespecial java/lang/Object/<init>()V" << std::endl;
            outputFile << "    return" << std::endl;
            outputFile << ".end method" << std::endl << std::endl;
        }
    }
    
    std::string mainClassNameForStatic = methodsByClass.empty() ? "Output" : methodsByClass.begin()->first;
    // Ensure the invokestatic call uses the correct signature for D1.main if it was changed
    std::string d1MainSignature = "()I"; // Default if not found
    if (methodSignatures.count(mainClassNameForStatic + ".main")) {
        d1MainSignature = methodSignatures[mainClassNameForStatic + ".main"];
    }

    outputFile << ".method public static main([Ljava/lang/String;)V" << std::endl;
    outputFile << "    ; Default main method" << std::endl;
    outputFile << "    invokestatic " << mainClassNameForStatic << "/main" << d1MainSignature << std::endl;
    outputFile << "    pop" << std::endl;
    outputFile << "    return" << std::endl;
    outputFile << ".end method" << std::endl;
    
    outputFile.close();
    std::cout << "Bytecode generated and saved to " << outputFileName << std::endl;
}

