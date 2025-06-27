#include "IRGenerator.h"
#include <iostream>
#include <cassert>

CFG IRGenerator::generate(Node* root) {
    // Reset state
    cfg = CFG();
    methodEntryPoints.clear();
    currentClass = "";
    currentMethod = "";
    
    if (!root) {
        std::cerr << "AST root is null!" << std::endl;
        return std::move(cfg);
    }

    // PASS 1: Collect all method entries first
    std::cout << "Pass 1: Collecting method entries..." << std::endl;
    collectMethodEntries(root);
    
    // Debug - print all found method entries
    std::cout << "Found " << methodEntryPoints.size() << " method entries:" << std::endl;
    for (const auto& entry : methodEntryPoints) {
        std::cout << "  " << entry.first << " -> block " << entry.second->label << std::endl;
    }
    
    // PASS 2: Generate IR for method bodies with knowledge of all methods
    std::cout << "Pass 2: Generating method bodies..." << std::endl;
    generateMethodBodies(root);
    
    std::cout << "Created " << cfg.blocks.size() << " blocks in CFG" << std::endl;
    return std::move(cfg);
}

// New method to collect all method entries in the first pass
void IRGenerator::collectMethodEntries(Node* node) {
    if (!node) return;
    
    if (node->type == "Goal") {
        for (auto child : node->children) {
            collectMethodEntries(child);
        }
    }
    else if (node->type == "MainClass") {
        std::string className = node->children.front()->value;
        std::string mainMethodName = className + ".main";
        
        // Create entry block for main method
        BasicBlock* mainBlock = cfg.createBlock();
        mainBlock->label = mainMethodName;
        methodEntryPoints[mainMethodName] = mainBlock;
        
        if (!cfg.entry) cfg.entry = mainBlock; // Set as entry if not already set
        
        std::cout << "  Registered main method: " << mainMethodName << std::endl;
    }
    else if (node->type == "ClassDeclaration") {
        std::string className = node->children.front()->value;
        std::string prevClass = currentClass;
        currentClass = className;
        
        // Find method declarations and create entry blocks for them
        for (auto child : node->children) {
            if (child->type == "MethodDeclarationList") {
                for (auto methodDecl : child->children) {
                    if (methodDecl->type == "MethodDeclaration") {
                        // Extract method name
                        std::string methodName = "";
                        for (auto methodChild : methodDecl->children) {
                            if (methodChild->type == "Identifier") {
                                methodName = methodChild->value;
                                break;
                            }
                        }
                        
                        if (!methodName.empty()) {
                            std::string fullMethodName = currentClass + "." + methodName;
                            BasicBlock* methodBlock = cfg.createBlock();
                            methodBlock->label = fullMethodName;
                            methodEntryPoints[fullMethodName] = methodBlock;
                            
                            std::cout << "  Registered method: " << fullMethodName << std::endl;
                        }
                    }
                }
            }
        }
        
        currentClass = prevClass;
    }
    else if (node->type == "ClassDeclarationList") {
        for (auto child : node->children) {
            collectMethodEntries(child);
        }
    }
}

//  generate IR for method bodies in the second pass
void IRGenerator::generateMethodBodies(Node* node) {
    if (!node) return;
    
    if (node->type == "Goal") {
        for (auto child : node->children) {
            generateMethodBodies(child);
        }
    }
    else if (node->type == "MainClass") {
        std::string className = node->children.front()->value;
        std::string mainMethodName = className + ".main";
        
        // Find the entry block for the main method
        auto mainIt = methodEntryPoints.find(mainMethodName);
        if (mainIt != methodEntryPoints.end()) {
            currentClass = className;
            currentMethod = "main";
            currentBlock = mainIt->second;
            
            // Process main method body
            for (auto child : node->children) {
                if (child->type == "Block" || child->type == "StatementList") {
                    processBlock(child);
                } else if (child->type == "PrintStatement") {
                    processPrintStatement(child);
                }
            }
            
            currentClass = "";
            currentMethod = "";
        }
    }
    else if (node->type == "ClassDeclaration") {
        std::string className = node->children.front()->value;
        std::string prevClass = currentClass;
        currentClass = className;
        
        // Find and process method bodies
        for (auto child : node->children) {
            if (child->type == "MethodDeclarationList") {
                for (auto methodDecl : child->children) {
                    if (methodDecl->type == "MethodDeclaration") {
                        std::string methodName = "";
                        std::string returnType = "";
                        
                        // Extract method name and return type
                        for (auto methodChild : methodDecl->children) {
                            if (methodChild->type == "Identifier") {
                                methodName = methodChild->value;
                            } else if (methodChild->type == "Type" || methodChild->type == "Int" || 
                                     methodChild->type == "Boolean" || methodChild->type == "IntArray") {
                                returnType = methodChild->value;
                            }
                        }
                        
                        if (!methodName.empty()) {
                            std::string fullMethodName = currentClass + "." + methodName;
                            
                            // Get the method's entry block
                            auto methodIt = methodEntryPoints.find(fullMethodName);
                            if (methodIt != methodEntryPoints.end()) {
                                std::string prevMethod = currentMethod;
                                currentMethod = methodName;
                                currentBlock = methodIt->second;
                                
                                // Process method body
                                for (auto bodyChild : methodDecl->children) {
                                    if (bodyChild->type == "MethodBody" || bodyChild->type == "Block") {
                                        processMethodBody(bodyChild);
                                    } else if (bodyChild->type == "Return") {
                                        processReturn(bodyChild);
                                    }
                                }
                                
                                
                                bool hasReturn = false;
                                for (const auto& instr : currentBlock->instructions) {
                                    if (instr.type == Instruction::RETURN) {
                                        hasReturn = true;
                                        break;
                                    }
                                }
                                
                                if (!hasReturn) {
                                    if (returnType == "int") {
                                        currentBlock->addInstruction(Instruction(Instruction::RETURN, "0"));
                                    } else if (returnType == "boolean") {
                                        currentBlock->addInstruction(Instruction(Instruction::RETURN, "false"));
                                    } else {
                                        currentBlock->addInstruction(Instruction(Instruction::RETURN, "null"));
                                    }
                                }
                                
                                currentMethod = prevMethod;
                            }
                        }
                    }
                }
            }
        }
        
        currentClass = prevClass;
    }
    else if (node->type == "ClassDeclarationList") {
        for (auto child : node->children) {
            generateMethodBodies(child);
        }
    }
}

// Helper method to find nodes of a specific type within a subtree
std::vector<Node*> IRGenerator::findNodesOfType(Node* root, const std::string& type) {
    std::vector<Node*> result;
    if (!root) return result;
    
    if (root->type == type) {
        result.push_back(root);
    }
    
    for (auto child : root->children) {
        auto childResults = findNodesOfType(child, type);
        result.insert(result.end(), childResults.begin(), childResults.end());
    }
    
    return result;
}

// helper method to generate proper method labels
std::string IRGenerator::getMethodFullName(const std::string& className, const std::string& methodName) {
    return className + "." + methodName;
}

void IRGenerator::processMethodDeclaration(Node* node) {
    if (node->children.size() < 4) return;
    
    // Find method attributes
    std::string methodName = "";
    std::string returnType = "";
    
    for (auto child : node->children) {
        if (child->type == "Identifier") {
            methodName = child->value;
        } else if (child->type == "Type" || child->type == "Int" || 
                   child->type == "Boolean" || child->type == "IntArray") {
            returnType = child->value;
        }
    }
    
    if (methodName.empty()) return;
    
    // method name
    std::string fullMethodName = currentClass + "." + methodName;
    
    // Create a new block for this method 
    BasicBlock* methodEntry = cfg.createBlock();
    methodEntry->label = fullMethodName;
    
    // Store method entry point
    methodEntryPoints[fullMethodName] = methodEntry;
    currentBlock = methodEntry;
    
    std::cout << "Processing method: " << fullMethodName << std::endl;
    
    // Save current method context
    std::string prevMethod = currentMethod;
    currentMethod = methodName;
    
    // Process method body
    for (auto child : node->children) {
        if (child->type == "MethodBody" || child->type == "Block") {
            processMethodBody(child);
        } else if (child->type == "Return") {
            processReturn(child);
        }
    }
    
    
    bool hasReturn = false;
    for (const auto& instr : currentBlock->instructions) {
        if (instr.type == Instruction::RETURN) {
            hasReturn = true;
            break;
        }
    }
    
    if (!hasReturn) {
        if (returnType == "int") {
            currentBlock->addInstruction(Instruction(Instruction::RETURN, "0"));
        } else if (returnType == "boolean") {
            currentBlock->addInstruction(Instruction(Instruction::RETURN, "false"));
        } else {
            currentBlock->addInstruction(Instruction(Instruction::RETURN, "null"));
        }
    }
    
    // Restore previous method context
    currentMethod = prevMethod;
}

std::string IRGenerator::processExpression(Node* node) {
    if (!node) return "";
    
    std::cout << "  Processing expression: " << node->type << std::endl;
    
    if (node->type == "IntegerLiteral") {
        return processIntegerLiteral(node);
    }
    else if (node->type == "BooleanLiteral") {
        return processBooleanLiteral(node);
    }
    else if (node->type == "Identifier") {
        return processIdentifier(node);
    }
    else if (node->type == "AddExpression" || node->type == "SubExpression" ||
             node->type == "MultExpression" || node->type == "DivExpression" ||
             node->type == "AndExpression" || node->type == "OrExpression" ||
             node->type == "LessThanExpression" || node->type == "GreaterThanExpression" ||
             node->type == "EqualExpression") {
        return processBinaryOp(node);
    }
    else if (node->type == "NotExpression") {
        return processUnaryOp(node);
    }
    else if (node->type == "ArrayAccess") {
        return processArrayAccess(node);
    }
    else if (node->type == "LengthExpression") {
        return processArrayLength(node);
    }
    else if (node->type == "MethodCall") {
        return processMethodCall(node);
    }
    else if (node->type == "NewObject") {
        return processNewObject(node);
    }
    else if (node->type == "NewArray" || node->type == "NewIntArray") {
        return processNewArray(node);
    }
    else if (node->type == "This" || node->type == "ThisExpression") {
        return processThisExpression(node);
    }
    
    // Process children for other expression types
    std::string result;
    for (auto child : node->children) {
        result = processExpression(child);
    }
    
    return result;
}

void IRGenerator::processStatement(Node* node) {
    if (!node) return;
    
    std::cout << "  Processing statement: " << node->type << std::endl;
    
    if (node->type == "IfStatement" || node->type == "IfElseStatement") {
        processIfStatement(node);
    }
    else if (node->type == "WhileStatement") {
        processWhileStatement(node);
    }
    else if (node->type == "Assignment") {
        processAssignment(node);
    }
    else if (node->type == "ArrayAssignment") {
        processArrayAssignment(node);
    }
    else if (node->type == "PrintStatement") {
        processPrintStatement(node);
    }
    else if (node->type == "Block") {
        processBlock(node);
    }
    else if (node->type == "StatementList") {
        // Process all statements in a statement list
        for (auto stmt : node->children) {
            processStatement(stmt);
        }
    }
    else {
        // For any other type, try processing it as an expression
        processExpression(node);
    }
}

void IRGenerator::processIfStatement(Node* node) {
    if (!node || node->children.size() < 2) {
        std::cerr << "Invalid if statement" << std::endl;
        return;
    }
    
    // Different handling based on if or if-else
    auto it = node->children.begin();
    auto condExpr = *it;         // First child - condition
    auto thenStmt = *(++it);     // Second child - then statement
    Node* elseStmt = nullptr;
    
    // Check if we have an else branch (for IfElseStatement)
    if (node->type == "IfElseStatement" && node->children.size() >= 3) {
        elseStmt = *(++it);
    }
    
    // Create blocks for the if structure
    BasicBlock* thenBlock = cfg.createBlock();
    BasicBlock* nextBlock = cfg.createBlock();
    BasicBlock* elseBlock = elseStmt ? cfg.createBlock() : nextBlock;
    
    // Process condition
    std::string condResult = processExpression(condExpr);

    // Add branch instructions
    currentBlock->addInstruction(Instruction(Instruction::IF_TRUE, thenBlock->label, condResult));
    currentBlock->addSuccessor(thenBlock);
    
    currentBlock->addInstruction(Instruction(Instruction::IF_FALSE, elseBlock->label, condResult));
    currentBlock->addSuccessor(elseBlock);
    
    // Process 'then' branch
    currentBlock = thenBlock;
    processStatement(thenStmt);
    currentBlock->addInstruction(Instruction(Instruction::GOTO, nextBlock->label));
    currentBlock->addSuccessor(nextBlock);
    
    // Process 'else' branch if it exists
    if (elseStmt) {
        currentBlock = elseBlock;
        processStatement(elseStmt);
        currentBlock->addInstruction(Instruction(Instruction::GOTO, nextBlock->label));
        currentBlock->addSuccessor(nextBlock);
    }
    
    // Continue with the next block
    currentBlock = nextBlock;
}

void IRGenerator::processWhileStatement(Node* node) {
    if (!node || node->children.size() < 2) {
        std::cerr << "Invalid while statement" << std::endl;
        return;
    }
    
    // Create blocks with descriptive names for the CFG
    BasicBlock* headerBlock = cfg.createBlock();
    BasicBlock* bodyBlock = cfg.createBlock();
    BasicBlock* exitBlock = cfg.createBlock();
    
    // Jump to the loop header from current block
    currentBlock->addInstruction(Instruction(Instruction::GOTO, headerBlock->label));
    currentBlock->addSuccessor(headerBlock);
    
    // Set current block to the loop header
    currentBlock = headerBlock;
    
    // Get condition and body
    auto it = node->children.begin();
    auto condExpr = *it;
    auto bodyStmt = *(++it);
    
    // Process condition
    std::string condResult = processExpression(condExpr);
    
    // Add conditional branches
    currentBlock->addInstruction(Instruction(Instruction::IF_TRUE, bodyBlock->label, condResult));
    currentBlock->addSuccessor(bodyBlock);
    
    currentBlock->addInstruction(Instruction(Instruction::IF_FALSE, exitBlock->label, condResult));
    currentBlock->addSuccessor(exitBlock);
    
    // Process loop body
    currentBlock = bodyBlock;
    processStatement(bodyStmt);
    
    // Add back-edge to the header
    currentBlock->addInstruction(Instruction(Instruction::GOTO, headerBlock->label));
    currentBlock->addSuccessor(headerBlock);
    
    // Continue with exit block
    currentBlock = exitBlock;
}

// Improve method body processing for BubbleSort methods
void IRGenerator::processMethodBody(Node* node) {
    std::cout << "Processing method body with " << node->children.size() << " children" << std::endl;
    
    for (auto child : node->children) {
        std::cout << "Method body child type: " << child->type << std::endl;
        
        if (child->type == "StatementList") {
            for (auto stmt : child->children) {
                processStatement(stmt);
            }
        }
        else if (child->type == "Return") {
            processReturn(child);
        }
        else if (child->type == "WhileStatement") {
            processWhileStatement(child);
        }
        else if (child->type == "IfStatement" || child->type == "IfElseStatement") {
            processIfStatement(child);
        }
        else if (child->type != "VarDeclaration" && child->type != "VarDeclarationList") {
            processStatement(child);
        }
    }
}

std::string IRGenerator::processIntegerLiteral(Node* node) {
    return node->value; // Just return the literal value
}

std::string IRGenerator::processBooleanLiteral(Node* node) {
    return node->value; // "true" or "false"
}

std::string IRGenerator::processIdentifier(Node* node) {
    return node->value; // Variable name
}

std::string IRGenerator::processBinaryOp(Node* node) {
    if (node->children.size() < 2) {
        std::cerr << "Invalid binary operation: not enough operands" << std::endl;
        return "";
    }
    
    auto it = node->children.begin();
    auto leftExpr = *it;
    auto rightExpr = *(++it);
    
    std::string leftResult = processExpression(leftExpr);
    std::string rightResult = processExpression(rightExpr);
    
    std::string temp = cfg.newTemp();
    std::string opcode;
    
    if (node->type == "AddExpression") opcode = "+";
    else if (node->type == "SubExpression") opcode = "-";
    else if (node->type == "MultExpression") opcode = "*";
    else if (node->type == "DivExpression") opcode = "/";
    else if (node->type == "AndExpression") opcode = "&&";
    else if (node->type == "OrExpression") opcode = "||";
    else if (node->type == "LessThanExpression") opcode = "<";
    else if (node->type == "GreaterThanExpression") opcode = ">";
    else if (node->type == "EqualExpression") opcode = "==";
    else opcode = "?"; // Unknown binary operator
    
    currentBlock->addInstruction(Instruction(Instruction::BINOP, temp, leftResult, opcode, rightResult));
    
    return temp;
}

std::string IRGenerator::processUnaryOp(Node* node) {
    if (node->children.empty()) {
        std::cerr << "Invalid unary operation: no operand" << std::endl;
        return "";
    }
    
    auto expr = node->children.front();
    std::string result = processExpression(expr);
    
    std::string temp = cfg.newTemp();
    std::string opcode = (node->type == "NotExpression") ? "!" : "-"; // Default to negation
    
    currentBlock->addInstruction(Instruction(Instruction::UNOP, temp, result, opcode));
    
    return temp;
}

std::string IRGenerator::processArrayAccess(Node* node) {
    if (node->children.size() < 2) {
        std::cerr << "Invalid array access: missing array or index" << std::endl;
        return "";
    }
    
    auto it = node->children.begin();
    auto arrayExpr = *it;
    auto indexExpr = *(++it);
    
    std::string array = processExpression(arrayExpr);
    std::string index = processExpression(indexExpr);
    
    std::string temp = cfg.newTemp();
    currentBlock->addInstruction(Instruction(Instruction::ARRAY_LOAD, temp, array, "", index));
    
    return temp;
}

std::string IRGenerator::processArrayLength(Node* node) {
    if (node->children.empty()) {
        std::cerr << "Invalid array length: no array" << std::endl;
        return "";
    }
    
    auto arrayExpr = node->children.front();
    std::string array = processExpression(arrayExpr);
    
    std::string temp = cfg.newTemp();
    currentBlock->addInstruction(Instruction(Instruction::CALL, temp, array + ".length"));
    
    return temp;
}


std::string IRGenerator::processMethodCall(Node* node) {
    std::string result_temp_name = cfg.newTemp();
    
    if (node->children.size() < 2) {
        std::cerr << "[DEBUG IRGen] Invalid method call structure. Node type: " << node->type << std::endl;
        return result_temp_name;
    }
    
    auto it = node->children.begin();
    Node* objAstNode = *it;
    ++it;
    Node* methodIdentifierNode = *it;
    
    std::string objRefIr = processExpression(objAstNode);
    std::string methodName = methodIdentifierNode->value;

    std::cerr << "[DEBUG IRGen] processMethodCall: objRefIr = " << objRefIr 
              << ", methodName = " << methodName 
              << ", objAstNode->type = " << objAstNode->type << std::endl;
    if (currentBlock) {
        std::cerr << "[DEBUG IRGen] Called from block: " << currentBlock->label << std::endl;
    }

    // Process parameters if any
    if (node->children.size() > 2) {
        ++it; 
        Node* argList = *it;
        for (auto argNode : argList->children) {
            std::string paramValueIr = processExpression(argNode);
            currentBlock->addInstruction(Instruction(Instruction::PARAM, paramValueIr));
        }
    }

    std::string calleeNameForInstruction = objRefIr + "." + methodName;
    currentBlock->addInstruction(Instruction(Instruction::CALL, result_temp_name, calleeNameForInstruction));
    
    std::string targetMethodFullName;

    if (objAstNode->type == "ThisExpression" || objAstNode->type == "This") {
        targetMethodFullName = currentClass + "." + methodName;
        std::cerr << "[DEBUG IRGen] 'this' call: currentClass = " << currentClass << ", targetMethodFullName = " << targetMethodFullName << std::endl;
    } else if (objAstNode->type == "NewObject") {
        if (!objAstNode->children.empty() && objAstNode->children.front()->type == "Identifier") {
            std::string classNameOfNewObj = objAstNode->children.front()->value;
            targetMethodFullName = classNameOfNewObj + "." + methodName;
            std::cerr << "[DEBUG IRGen] 'new' call: classNameOfNewObj = " << classNameOfNewObj << ", targetMethodFullName = " << targetMethodFullName << std::endl;
        } else {
            std::cerr << "[DEBUG IRGen] Warning: NewObject AST node has unexpected structure. Children count: " << objAstNode->children.size() << std::endl;
            if (!objAstNode->children.empty()) {
                 std::cerr << "[DEBUG IRGen] NewObject child type: " << objAstNode->children.front()->type << std::endl;
            }
        }
    } else if (objAstNode->type == "Identifier") {
        std::cerr << "[DEBUG IRGen] Call on identifier '" << objAstNode->value << "'. Type: " << objAstNode->type << ". Complex case, requires type lookup." << std::endl;
        
    } else {
        std::cerr << "[DEBUG IRGen] Unhandled objAstNode->type for method call: " << objAstNode->type << std::endl;
    }

    if (!targetMethodFullName.empty()) {
        std::cerr << "[DEBUG IRGen] Attempting to find method entry point for: " << targetMethodFullName << std::endl;


        auto targetIt = methodEntryPoints.find(targetMethodFullName);
        if (targetIt != methodEntryPoints.end()) {
            currentBlock->callTargets[targetMethodFullName] = targetIt->second;
            std::cerr << "[DEBUG IRGen] Found method entry point. Added to callTargets for block " << currentBlock->label << " -> " << targetIt->second->label << std::endl;
        } else {
            std::cerr << "[DEBUG IRGen] Warning: Could not find method entry point for call to " << targetMethodFullName 
                      << " from block " << (currentBlock ? currentBlock->label : "unknown_block") 
                      << " (AST obj type: " << objAstNode->type << ", IR obj ref: " << objRefIr << ")" << std::endl;
        }
    } else {
        std::cerr << "[DEBUG IRGen] Warning: targetMethodFullName is empty for call " << calleeNameForInstruction
                  << " (AST obj type: " << objAstNode->type << ")"
                  << " from block " << (currentBlock ? currentBlock->label : "unknown_block") << std::endl;
    }
    
    return result_temp_name;
}

std::string IRGenerator::processNewObject(Node* node) {
    if (node->children.empty()) {
        std::cerr << "Invalid new object: missing class name" << std::endl;
        return "";
    }
    
    auto classNameNode = node->children.front();
    std::string className = classNameNode->value;
    
    std::string temp = cfg.newTemp();
    currentBlock->addInstruction(Instruction(Instruction::CALL, temp, "new " + className));
    
    return temp;
}

std::string IRGenerator::processNewArray(Node* node) {
    std::string size = "0"; // Default size
    
    if (!node->children.empty()) {
        auto it = node->children.begin();
        if (node->type == "NewIntArray" && node->children.size() >= 3) {
            // Skip to the third child for array size
            ++it; ++it;
            size = processExpression(*it);
        } else {
            // Just use the first child for size
            size = processExpression(*it);
        }
    }
    
    std::string temp = cfg.newTemp();
    currentBlock->addInstruction(Instruction(Instruction::CALL, temp, "new int[" + size + "]"));
    
    return temp;
}

std::string IRGenerator::processThisExpression(Node* node) {
    return "this"; // Just return "this"
}

void IRGenerator::processAssignment(Node* node) {
    if (node->children.size() < 2) {
        std::cerr << "Invalid assignment: missing left side or right side" << std::endl;
        return;
    }
    
    auto it = node->children.begin();
    auto leftNode = *it;
    auto rightNode = *(++it);
    
    std::string leftSide = leftNode->value; // Assuming left side is always an identifier
    std::string rightSide = processExpression(rightNode);
    
    currentBlock->addInstruction(Instruction(Instruction::ASSIGN, leftSide, rightSide));
}

void IRGenerator::processArrayAssignment(Node* node) {
    if (node->children.size() < 3) {
        std::cerr << "Invalid array assignment: missing array, index, or value" << std::endl;
        return;
    }
    
    auto it = node->children.begin();
    auto arrayNode = *it;
    auto indexNode = *(++it);
    auto valueNode = *(++it);
    
    std::string array = arrayNode->value; // Assuming array is always an identifier
    std::string index = processExpression(indexNode);
    std::string value = processExpression(valueNode);
    
    currentBlock->addInstruction(Instruction(Instruction::ARRAY_STORE, array, index, "", value));
}

void IRGenerator::processReturn(Node* node) {
    if (node->children.empty()) {
        // Void return
        currentBlock->addInstruction(Instruction(Instruction::RETURN, ""));
        return;
    }
    
    auto expr = node->children.front();
    std::string result = processExpression(expr);
    currentBlock->addInstruction(Instruction(Instruction::RETURN, result));
}

void IRGenerator::processBlock(Node* node) {
    if (!node || node->children.empty()) return;
    
    std::cout << "Processing block with " << node->children.size() << " children" << std::endl;
    
    // Process each statement in the block
    for (auto child : node->children) {
        if (child->type == "PrintStatement") {
            processPrintStatement(child);
        } 
        else if (child->type == "StatementList") {
            // Process each statement in the statement list
            for (auto stmt : child->children) {
                processStatement(stmt);
            }
        }
        else {
            processStatement(child);
        }
    }
}


void IRGenerator::processPrintStatement(Node* node) {
    if (node->children.empty()) {
        std::cerr << "Invalid print statement: nothing to print" << std::endl;
        return;
    }
    
    std::cout << "Processing print statement with " << node->children.size() << " children" << std::endl;
    
    // Make sure we have a current block
    if (!currentBlock) {
        currentBlock = cfg.createBlock();
    }
    
    auto expr = node->children.front();
    std::string result = processExpression(expr);
    
    std::cout << "Print expression result: " << result << std::endl;
    
    // Generate a call to System.out.println
    currentBlock->addInstruction(Instruction(Instruction::PARAM, result));
    currentBlock->addInstruction(Instruction(Instruction::CALL, "", "System.out.println"));
}

//CFG.generateDotFile method to show method call relationships
void CFG::generateDotFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    outFile << "digraph {" << std::endl;
    outFile << "  graph [ splines = ortho ];" << std::endl;
    outFile << "  node [ shape = box ];" << std::endl;
    
    // Generate nodes for basic blocks
    for (const auto& block : blocks) {
        // Skip empty blocks
        if (block->instructions.empty()) continue;
        
        outFile << "  \"" << block->label << "\" [ label = \" " << block->label << " \\n" 
                << block->toString() << " \" ];" << std::endl;
    }
    
    // Generate edges between blocks with condition labels
    for (const auto& block : blocks) {
        // Skip empty blocks
        if (block->instructions.empty()) continue;
        
        // Regular control flow edges
        for (const auto& succ : block->successors) {
            bool hasLabel = false;
            std::string edgeLabel;
            
            // Check if this is a conditional edge
            for (const auto& instr : block->instructions) {
                if (instr.type == Instruction::IF_TRUE && instr.result == succ->label) {
                    hasLabel = true;
                    edgeLabel = "true";
                    break;
                } 
                else if (instr.type == Instruction::IF_FALSE && instr.result == succ->label) {
                    hasLabel = true;
                    edgeLabel = "false";
                    break;
                }
            }
            
            // Draw edges with appropriate labels
            if (hasLabel) {
                outFile << "  \"" << block->label << "\" -> \"" << succ->label 
                        << "\" [ xlabel = \" " << edgeLabel << " \" ];" << std::endl;
            } else {
                outFile << "  \"" << block->label << "\" -> \"" << succ->label << "\";" << std::endl;
            }
        }
        
        // Method call edges (dashed lines to show calls)
        for (const auto& callTarget : block->callTargets) {
            outFile << "  \"" << block->label << "\" -> \"" << callTarget.second->label 
                    << "\" [ style = dashed, color = blue, label = \"call\" ];" << std::endl;
        }
    }
    
    // Add entry node
    if (entry && !entry->instructions.empty()) {
        outFile << "  entry [shape=oval, label=\"entry\"];" << std::endl;
        outFile << "  entry -> \"" << entry->label << "\";" << std::endl;
    }
    
    outFile << "}" << std::endl;
    outFile.close();
    
    std::cout << "CFG visualization generated at " << filename << std::endl;
}