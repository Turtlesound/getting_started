#include "IRGenerator.h"
#include <iostream>
#include <cassert>

CFG IRGenerator::generate(Node* root) {
    // Reset the CFG
    cfg = CFG();
    
    // Create entry block
    BasicBlock* programEntry = cfg.createBlock();
    currentBlock = programEntry;
    
    // Process the AST
    if (root) {
        std::cout << "Starting AST traversal from root node: " << root->type << std::endl;
        traverseAST(root);
    } else {
        std::cerr << "AST root is null!" << std::endl;
    }
    
    std::cout << "Created " << cfg.blocks.size() << " blocks in CFG" << std::endl;
    return std::move(cfg);
}

void IRGenerator::traverseAST(Node* node) {
    if (!node) return;
    
    std::cout << "Processing node: " << node->type << std::endl;
    
    // Handle different node types
    if (node->type == "Goal") {
        for (auto child : node->children) {
            traverseAST(child);
        }
    }
    else if (node->type == "MainClass") {
        std::string className = node->children.front()->value;
        currentClass = className;
        std::cout << "Processing main class: " << className << std::endl;
        
        // Process other children to find statements
        for (auto child : node->children) {
            if (child->type == "PrintStatement" || 
                child->type == "Statement" || 
                child->type == "StatementList" ||
                child->type == "WhileStatement" ||  // Added for loop handling
                child->type == "IfStatement" ||     // Added for if handling
                child->type == "IfElseStatement") { // Added for if-else handling
                processStatement(child);
            } else {
                traverseAST(child);
            }
        }
        currentClass = "";
    }
    else if (node->type == "ClassDeclaration") {
        std::string className = node->children.front()->value;
        currentClass = className;
        std::cout << "Processing class: " << className << std::endl;
        
        // Find method declarations
        for (auto child : node->children) {
            if (child->type == "MethodDeclarationList") {
                for (auto methodDecl : child->children) {
                    if (methodDecl->type == "MethodDeclaration") {
                        processMethodDeclaration(methodDecl);
                    }
                }
            }
        }
        currentClass = "";
    }
    else if (node->type == "ClassDeclarationList") {
        // Process all class declarations
        for (auto child : node->children) {
            traverseAST(child);
        }
    }
    else {
        // Continue traversing for other node types
        for (auto child : node->children) {
            traverseAST(child);
        }
    }
}

void IRGenerator::processMethodDeclaration(Node* node) {
    if (!node) return;
    
    // Extract method name and parameters
    std::string methodName;
    for (auto child : node->children) {
        if (child->type == "Identifier") {
            methodName = child->value;
            break;
        }
    }
    
    currentMethod = methodName;
    std::cout << "Processing method: " << currentMethod << std::endl;
    
    // Create entry block for this method
    BasicBlock* methodEntry = cfg.createBlock();
    methodEntry->label = "block_" + std::to_string(methodEntry->blockId);
    
    // If there's an existing block, link to the method entry
    if (currentBlock && currentBlock != methodEntry) {
        currentBlock->addInstruction(Instruction(Instruction::GOTO, methodEntry->label));
        currentBlock->addSuccessor(methodEntry);
    }
    
    currentBlock = methodEntry;
    
    // Process parameters
    for (auto child : node->children) {
        if (child->type == "ParameterList") {
            for (auto param : child->children) {
                if (param->type == "Parameter") {
                    // Find parameter identifier
                    for (auto paramChild : param->children) {
                        if (paramChild->type == "Identifier") {
                            currentBlock->addInstruction(Instruction(Instruction::PARAM, paramChild->value));
                        }
                    }
                }
            }
        }
    }
    
    // Process method body
    for (auto child : node->children) {
        if (child->type == "MethodBody") {
            // Process statements in method body
            for (auto bodyChild : child->children) {
                if (bodyChild->type == "StatementList") {
                    for (auto stmt : bodyChild->children) {
                        processStatement(stmt);
                    }
                }
                // Process return statement
                if (bodyChild->type == "Return") {
                    processReturn(bodyChild);
                }
            }
        }
    }
    
    currentMethod = "";
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

// Keep the existing methods for processIntegerLiteral, etc.
// ...

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

// Add these implementations to the end of the file:

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
    if (node->children.size() < 2) {
        std::cerr << "Invalid method call: missing object or method name" << std::endl;
        return "";
    }
    
    auto it = node->children.begin();
    auto objExpr = *it;
    auto methodNameNode = *(++it);
    
    std::string obj = processExpression(objExpr);
    std::string methodName = methodNameNode->value;
    
    // Process arguments if present
    std::vector<std::string> args;
    if (node->children.size() > 2) {
        auto argListIt = ++(++it);
        if (argListIt != node->children.end()) {
            auto argList = *argListIt;
            for (auto argExpr : argList->children) {
                args.push_back(processExpression(argExpr));
            }
        }
    }
    
    // Generate param instructions
    for (const auto& arg : args) {
        currentBlock->addInstruction(Instruction(Instruction::PARAM, arg));
    }
    
    // Generate call instruction
    std::string temp = cfg.newTemp();
    currentBlock->addInstruction(Instruction(Instruction::CALL, temp, obj + "." + methodName));
    
    return temp;
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
    for (auto child : node->children) {
        processStatement(child);
    }
}


void IRGenerator::processPrintStatement(Node* node) {
    if (node->children.empty()) {
        std::cerr << "Invalid print statement: nothing to print" << std::endl;
        return;
    }
    
    auto expr = node->children.front();
    std::string result = processExpression(expr);
    
    // Generate a call to System.out.println
    currentBlock->addInstruction(Instruction(Instruction::PARAM, result));
    currentBlock->addInstruction(Instruction(Instruction::CALL, "", "System.out.println"));
}