#include "SemanticAnalyzer.h"
#include <iostream>
#include <algorithm>

SemanticAnalyzer::SemanticAnalyzer(const SymbolTable& st) : symbolTable(st) {}

const std::vector<std::string>& SemanticAnalyzer::getErrors() const {
    return errors;
}

void SemanticAnalyzer::analyze(Node* node) {
    if (!node) return;
    
    // Check node type first
    if (node->type == "ClassDeclaration" && !node->children.empty()) {
        std::string className = node->children.front()->value;
        checkClassDeclaration(node); // Check BEFORE setting context
        currentClass = className;
    }
    else if (node->type == "MainClass" && !node->children.empty()) { 
        std::string className = node->children.front()->value;
        currentClass = className;
        // Check for duplicate class AFTER setting context
        checkClassDeclaration(node);
    }
    else if (node->type == "MethodDeclaration") {
        auto it = node->children.begin();
        if (it != node->children.end() && (*it)->type == "Public") {
            ++it;
        }
        ++it; // Skip return type
        if (it != node->children.end()) {
            std::string methodName = (*it)->value;
            currentMethod = methodName;
            // Check for duplicate method AFTER setting context
            checkMethodDeclaration(node);
        }
        
        // For method bodies, first collect all variable declarations
        // to properly handle variable-before-declaration checks
        for (auto child : node->children) {
            if (child->type == "MethodBody") {
                // First pass: process all declarations
                collectDeclarations(child);
            }
        }
    }
    else if (node->type == "VarDeclaration") {
        // Check if the type is a valid class
        std::string typeStr = node->children.front()->value;
        if (isClassType(typeStr) && typeStr != currentClass && !symbolTable.isClassDeclared(typeStr)) {
            addError("'" + typeStr + "' is undefined", node->lineno);
        }
        
        if (!currentMethod.empty()) {
            checkLocalVarDeclaration(node);
        } else if (!currentClass.empty()) {
            checkFieldDeclaration(node);
        }
    }

    else if (node->type == "MethodCall") {
        checkMethodCall(node);
    }
    else if (node->type == "Assignment") {
        checkAssignment(node);
    }
    else if (node->type == "ArrayAccess") {
        checkArrayAccess(node);
    }
    else if (node->type == "ArrayAssignment") {
        checkArrayAssignment(node);
    }
    else if (node->type == "VarDeclaration" && node->children.size() >= 2) {
        checkLocalVarDeclaration(node);
    }
    else if (node->type == "Return") {
        checkReturnType(node);
    }
    else if (node->type == "If" || node->type == "While") {
        checkConditions(node);
    }
    
    // Save current context
    std::string savedClass = currentClass;
    std::string savedMethod = currentMethod;
    
    // Process all children
    for (auto child : node->children) {
        analyze(child);
    }
    
    // Restore context
    currentClass = savedClass;
    currentMethod = savedMethod;
}

void SemanticAnalyzer::checkClassDeclaration(Node* node) {
    if (!node || node->children.empty()) return;
    
    std::string className = node->children.front()->value;
    
    // duplicate check with direct map lookup
    if (classLines.count(className) > 0) {
        
        addError("Already Declared Class: '" + className + "'", node->lineno);
    } else {
        classLines[className] = node->lineno;
    }
}

void SemanticAnalyzer::checkFieldDeclaration(Node* node) {
    if (!node || node->children.size() < 2) return;
    
    std::string fieldName = node->children.back()->value;
    std::string key = currentClass + "." + fieldName;
    
    if (fieldLines.find(key) != fieldLines.end()) {
        // This is a duplicate field declaration
        addError("Already Declared variable: '" + fieldName + "'", node->lineno);
    } else {
        fieldLines[key] = node->lineno;
    }
}

void SemanticAnalyzer::checkMethodDeclaration(Node* node) {
    if (!node || node->children.size() < 3) return;
    
    auto it = node->children.begin();
    if ((*it)->type == "Public") ++it;
    ++it; 
    
    std::string methodName = (*it)->value;
    std::string key = currentClass + "." + methodName;
    
    if (methodLines.find(key) != methodLines.end()) {
        // This is a duplicate method declaration
        addError("Already Declared Function: '" + methodName + "'", node->lineno);
    } else {
        methodLines[key] = node->lineno;
    }
    
    // Check parameters for duplicates within this method
    ++it; // Move to parameter list
    if (it != node->children.end() && (*it)->type == "ParameterList") {
        std::cout << "DEBUG: Parameters for method " << methodName << ": ";
        for (auto paramNode : (*it)->children) {
            if (paramNode->type == "Parameter" && paramNode->children.size() >= 2) {
                std::cout << paramNode->children.back()->value << " ";
            }
        }
        std::cout << std::endl;
        std::map<std::string, int> paramNames;
        
        for (auto paramNode : (*it)->children) {
            if (paramNode->type == "Parameter" && paramNode->children.size() >= 2) {
                std::string paramName = paramNode->children.back()->value;
                
                if (paramNames.find(paramName) != paramNames.end()) {
                    // Duplicate parameter in the same method
                    addError("Already Declared parameter: '" + paramName + "'", paramNode->lineno);
                } else {
                    paramNames[paramName] = paramNode->lineno;
                    
                    // Add to global param tracking
                    std::string paramKey = key + "." + paramName;
                    paramLines[paramKey] = paramNode->lineno;
                }
            }
        }
    }
}

void SemanticAnalyzer::checkLocalVarDeclaration(Node* node) {
    if (!node || node->children.size() < 2) return;
    
    std::string varName = node->children.back()->value;
    std::string methodKey = currentClass + "." + currentMethod;
    std::string paramKey = methodKey + "." + varName;
    
    // Check if it conflicts with a parameter
    if (paramLines.find(paramKey) != paramLines.end()) {
        addError("Already Declared parameter: '" + varName + "'", node->lineno);
        return; // Return early to avoid adding it to localLines
    }
    
    // Check if it's a duplicate local in the SAME class and method
    std::string localKey = methodKey + "." + varName;
    
    // Check if this is a true duplicate (previously declared in the same scope)
    // if it was used before declaration (should be handled in checkAssignment)
    if (localLines.find(localKey) != localLines.end()) {
        //  already recorded this variable and the current line is not
        // the same as the recorded line, then it's a duplicate declaration
        if (localLines[localKey] != node->lineno) {
            addError("Already Declared variable: '" + varName + "'", node->lineno);
        }
        // Otherwise, it's just the variable we're processing now
    } else {
        // New variable declaration - record it
        localLines[localKey] = node->lineno;
    }
}


bool SemanticAnalyzer::isIntegerType(const std::string& type) const {
    return type == "int";
}

bool SemanticAnalyzer::isBooleanType(const std::string& type) const {
    return type == "boolean";
}

bool SemanticAnalyzer::isArrayType(const std::string& type) const {
    return type.size() > 2 && type.substr(type.size() - 2) == "[]";
}

bool SemanticAnalyzer::isClassType(const std::string& type) const {
    return !isIntegerType(type) && !isBooleanType(type) && !isArrayType(type) && type != "void";
}

bool SemanticAnalyzer::areTypesCompatible(const std::string& expectedType, const std::string& actualType) const {
    // Direct match
    if (expectedType == actualType) {
        return true;
    }
    
    // Return types must match exactly - no implicit conversions allowed
    return false;
}


std::string SemanticAnalyzer::getExpressionName(Node* node) {
    if (!node) return "unknown";
    
    // For simple identifiers, return the value
    if (node->type == "Identifier") {
        return node->value;
    }
    // For literals, return the actual literal value
    else if (node->type == "IntegerLiteral") {
        return node->value;
    } 
    else if (node->type == "BooleanLiteral") {
        return node->value;
    }
    // For method calls, get method name
    else if (node->type == "MethodCall" && node->children.size() >= 2) {
        auto methodNameNode = *(++node->children.begin());
        return getExpressionName(node->children.front()) + "." + methodNameNode->value + "()";
    }
    // Default case
    return "expression";
}


std::string SemanticAnalyzer::getExpressionType(Node* node) {
    if (!node) return "";

    // Handle different node types
    if (node->type == "IntegerLiteral") {
        return "int";
    }
    else if (node->type == "BooleanLiteral") {
        return "boolean";
    }
    else if (node->type == "StringLiteral") {
        return "String";
    }
    else if (node->type == "This") {  
        return currentClass;
    }
    else if (node->type == "ThisExpression") {
        return currentClass;
    }
    else if (node->type == "NewObject" && !node->children.empty()) {
        // For expressions like "new ClassName()"
        auto classNameNode = node->children.front();
        if (classNameNode->type == "Identifier") {
            std::string className = classNameNode->value;
            if (!symbolTable.isClassDeclared(className)) {
                addError("'" + className + "' is undefined", node->lineno);
                return "";
            }
            return className;
        }
        return "";
    }
    else if (node->type == "Identifier") {
        std::string identifierName = node->value;
        
        // First check if identifier is a parameter or local variable in current method
        if (!currentClass.empty() && !currentMethod.empty()) {
            auto classRecord = symbolTable.getClass(currentClass);
            if (classRecord) {
                auto methodIt = classRecord->methods.find(currentMethod);
                if (methodIt != classRecord->methods.end()) {
                    auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
                    if (methodRecord) {
                        // Check parameters first
                        auto paramIt = methodRecord->params.find(identifierName);
                        if (paramIt != methodRecord->params.end()) {
                            auto varRecord = std::dynamic_pointer_cast<VarRecord>(paramIt->second);
                            if (varRecord) {
                                return varRecord->type;
                            }
                        }
                        
                        // Then check local variables - ensure they're declared before use
                        auto localIt = methodRecord->locals.find(identifierName);
                        if (localIt != methodRecord->locals.end()) {
                            // Check if the variable is used before declaration
                            std::string localKey = currentClass + "." + currentMethod + "." + identifierName;
                            if (localLines.find(localKey) != localLines.end() && 
                                node->lineno < localLines[localKey]) {
                                addError("'" + identifierName + "' is not defined yet", node->lineno);
                                return "";
                            }
                            auto varRecord = std::dynamic_pointer_cast<VarRecord>(localIt->second);
                            if (varRecord) {
                                return varRecord->type;
                            }
                        }
                    }
                }
            }
        }
        
        // Then check fields of the current class
        if (!currentClass.empty()) {
            auto classRecord = symbolTable.getClass(currentClass);
            if (classRecord) {
                auto fieldIt = classRecord->fields.find(identifierName);
                if (fieldIt != classRecord->fields.end()) {
                    auto varRecord = std::dynamic_pointer_cast<VarRecord>(fieldIt->second);
                    if (varRecord) {
                        return varRecord->type;
                    }
                }
            }
        }
        
        // Finally, check if it's a class name
        if (symbolTable.isClassDeclared(identifierName)) {
            return identifierName;
        }
        
        // Not found
        addError("'" + identifierName + "' does not exist in the current scope", node->lineno);
        return "";
    }
    else if (node->type == "MethodCall" && node->children.size() >= 2) {
        auto objExpr = node->children.front();
        auto methodNameNode = *(++node->children.begin());
        std::string objType = getExpressionType(objExpr);
        if (objType.empty()) return "";

        auto classRecord = symbolTable.getClass(objType);
        if (!classRecord) {
            addError("method call on non-class type '" + objType + "'", node->lineno);
            return "";
        }

        std::string methodName = methodNameNode->value;
        auto methodIt = classRecord->methods.find(methodName);
        if (methodIt == classRecord->methods.end()) {
            addError("'" + methodName + "' does not exist in class '" + objType + "'", node->lineno);
            return "";
        }

        auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
        if (methodRecord) {
            // Validate method parameters while evaluating the expression type
            checkMethodCall(node);
            return methodRecord->returnType;
        }
        return "";
    }
    else if (node->type == "ArrayAccess" && node->children.size() >= 2) {
        auto arrayExpr = node->children.front();
        std::string arrayType = getExpressionType(arrayExpr);
        
        // Check if array index is an integer
        auto indexExpr = *(++node->children.begin());
        std::string indexType = getExpressionType(indexExpr);
        if (!indexType.empty() && indexType != "int") {
            addError("array index must be an integer, got '" + indexType + "'", indexExpr->lineno);
        }
        
        // If it's an array type, return the element type
        if (isArrayType(arrayType)) {
            return arrayType.substr(0, arrayType.length() - 2); 
        } else {
            addError("array access on non-array type '" + arrayType + "'", node->lineno);
            return "";
        }
    }
    else if (node->type == "LengthExpression" && !node->children.empty()) {
        auto arrayExpr = node->children.front();
        std::string arrayType = getExpressionType(arrayExpr);
        
        if (!isArrayType(arrayType)) {
            addError("member .length used on non-array type '" + arrayType + "'", node->lineno);
            return "";
        }
        
        return "int";
    }
    else if (node->type == "NewArray" && node->children.size() >= 1) {
        // Check that array size is an integer
        auto sizeExpr = node->children.front();
        std::string sizeType = getExpressionType(sizeExpr);
        
        if (!sizeType.empty() && sizeType != "int") {
            addError("array size must be an integer, got '" + sizeType + "'", sizeExpr->lineno);
        }
        
        return "int[]"; // Default to int array
    }
    else if (node->type == "NewIntArray" && node->children.size() >= 3) {
        // Get the third child using iterator
        auto it = node->children.begin();
        std::advance(it, 2); // Advance to third element (index 2)
        auto sizeExpr = *it;
        
        std::string sizeType = getExpressionType(sizeExpr);
        
        if (!sizeType.empty() && sizeType != "int") {
            addError("array size must be an integer, got '" + sizeType + "'", sizeExpr->lineno);
        }
        
        return "int[]"; 
    }
    else if (node->type == "AddExpression" || node->type == "SubExpression" || 
             node->type == "MultExpression" || node->type == "DivExpression" || 
             node->type == "ModExpression") {
        if (node->children.size() >= 2) {
            auto leftExpr = node->children.front();
            auto rightExpr = *(++node->children.begin());
            
            std::string leftType = getExpressionType(leftExpr);
            std::string rightType = getExpressionType(rightExpr);
            
            if (leftType != "int" || rightType != "int") {
                addError("arithmetic operation requires integer operands", node->lineno);
                return "";
            }
            
            return "int";
        }
        return "";
    }
    else if (node->type == "LessThanExpression" || node->type == "GreaterThanExpression") {
        if (node->children.size() >= 2) {
            auto leftExpr = node->children.front();
            auto rightExpr = *(++node->children.begin());
            
            std::string leftType = getExpressionType(leftExpr);
            std::string rightType = getExpressionType(rightExpr);
            
            if (leftType != "int" || rightType != "int") {
                addError("comparison operation requires integer operands", node->lineno);
                return "";
            }
            
            return "boolean";
        }
        return "";
    }
    else if (node->type == "EqualExpression") {
        if (node->children.size() >= 2) {
            auto leftExpr = node->children.front();
            auto rightExpr = *(++node->children.begin());
            
            std::string leftType = getExpressionType(leftExpr);
            std::string rightType = getExpressionType(rightExpr);
            
            if (leftType != rightType) {
                addError("equality comparison requires operands of same type", node->lineno);
                return "";
            }
            
            return "boolean";
        }
        return "";
    }
    else if (node->type == "AndExpression" || node->type == "OrExpression") {
        if (node->children.size() >= 2) {
            auto leftExpr = node->children.front();
            auto rightExpr = *(++node->children.begin());
            
            std::string leftType = getExpressionType(leftExpr);
            std::string rightType = getExpressionType(rightExpr);
            
            if (leftType != "boolean" || rightType != "boolean") {
                addError("logical operation requires boolean operands", node->lineno);
                return "";
            }
            
            return "boolean";
        }
        return "";
    }
    else if (node->type == "NotExpression" && !node->children.empty()) {
        auto expr = node->children.front();
        std::string exprType = getExpressionType(expr);
        
        if (exprType != "boolean") {
            addError("logical not operation requires boolean operand", node->lineno);
            return "";
        }
        
        return "boolean";
    }
    
    // For any other node types, recursively process children
    for (auto child : node->children) {
        getExpressionType(child);
    }
    
    return "";
}

void SemanticAnalyzer::checkMethodCall(Node* node) {
    if (!node || node->type != "MethodCall" || node->children.size() < 2) return;
    
    auto objExpr = node->children.front();
    auto methodNameNode = *(++node->children.begin());
    
    std::string objType = getExpressionType(objExpr);
    if (objType.empty()) return; // Error already reported 
    
    auto classRecord = symbolTable.getClass(objType);
    if (!classRecord) {
        addError("method call on non-class type '" + objType + "'", node->lineno);
        return;
    }
    
    std::string methodName = methodNameNode->value;
    auto methodIt = classRecord->methods.find(methodName);
    if (methodIt == classRecord->methods.end()) {
        if (objExpr->type == "Identifier") {
            addError("'" + methodName + "' does not exist", node->lineno);
        } else {
            addError("undefined method '" + methodName + "' in class '" + objType + "'", node->lineno);
        }
        return;
    }
    
    // Check argument count and types
    auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
    if (!methodRecord) return;
    
    
    size_t expectedParamCount = methodRecord->paramOrder.size();
    size_t actualArgCount = 0;
    std::vector<Node*> argNodes;
    
    // Handle argument counting based on node structure
    if (node->children.size() > 2) {
        auto thirdChild = *(++(++node->children.begin()));
        
        if (thirdChild->type == "ExpressionList") {
            // Count the actual expressions in the ExpressionList
            actualArgCount = thirdChild->children.size();
            
            // Copy argument nodes for type checking
            for (auto& arg : thirdChild->children) {
                argNodes.push_back(arg);
            }
        } else {
            // No ExpressionList - count remaining children
            actualArgCount = node->children.size() - 2;
            
            // Add all remaining nodes as arguments
            auto it = node->children.begin();
            std::advance(it, 2);
            while (it != node->children.end()) {
                argNodes.push_back(*it);
                ++it;
            }
        }
    } else {
        // Just object and method name - zero arguments
        actualArgCount = 0;
    }
    
    // Now check argument count
    if (expectedParamCount != actualArgCount) {
        addError("method '" + methodName + "' expects " + std::to_string(expectedParamCount) + 
                 " arguments, but got " + std::to_string(actualArgCount), node->lineno);
        return;
    }
    
    // Check each argument type against parameter type
    if (!argNodes.empty()) {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        
        // Collect parameters in declaration order
        for (const auto& paramName : methodRecord->paramOrder) {
            auto it = methodRecord->params.find(paramName);
            if (it != methodRecord->params.end()) {
                auto varRecord = std::dynamic_pointer_cast<VarRecord>(it->second);
                if (varRecord) {
                    paramTypes.push_back(varRecord->type);
                    paramNames.push_back(varRecord->name);
                }
            }
        }
        
        // Check each argument against corresponding parameter
        for (size_t i = 0; i < argNodes.size() && i < paramTypes.size(); ++i) {
            std::string argType = getExpressionType(argNodes[i]);
            if (argType.empty()) continue; // Error already reported
            
            if (!areTypesCompatible(paramTypes[i], argType)) {
                std::string argName = getExpressionName(argNodes[i]);
                addError("invalid parameter '" + argName + "' for method '" + methodName + 
                         "': expected '" + paramTypes[i] + "', got '" + argType + "'", 
                         argNodes[i]->lineno);
            }
        }
    }
}

void SemanticAnalyzer::checkAssignment(Node* node) {
    if (!node || node->type != "Assignment" || node->children.size() < 2) return;
    
    auto leftNode = node->children.front();
    auto rightNode = *(++node->children.begin());
    
    // First check if we're using a variable before it's declared
    if (leftNode->type == "Identifier") {
        std::string varName = leftNode->value;
        std::string methodKey = currentClass + "." + currentMethod;
        std::string localKey = methodKey + "." + varName;
        
        // Check if this is a local variable (not a field or parameter)
        bool isLocal = false;
        bool existsAsField = false;
        
        // First check if it's a field in the current class
        if (!currentClass.empty()) {
            auto classRecord = symbolTable.getClass(currentClass);
            if (classRecord && classRecord->fields.find(varName) != classRecord->fields.end()) {
                existsAsField = true;
            }
        }
        
        // Then check if it's a local variable
        if (!existsAsField && !currentClass.empty() && !currentMethod.empty()) {
            auto classRecord = symbolTable.getClass(currentClass);
            if (classRecord) {
                auto methodIt = classRecord->methods.find(currentMethod);
                if (methodIt != classRecord->methods.end()) {
                    auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
                    // Skip parameters - they're always in scope
                    if (methodRecord && methodRecord->params.find(varName) == methodRecord->params.end() && 
                        methodRecord->locals.find(varName) != methodRecord->locals.end()) {
                        isLocal = true;
                    }
                }
            }
        }

        // Only check "used before declared" for local variables, not fields or parameters
        if (isLocal && localLines.find(localKey) != localLines.end() && 
            node->lineno < localLines[localKey]) {
            addError("'" + varName + "' is not defined yet", node->lineno);
            return; // Return early after detecting an error
        }
    }
    
    // Continue with type checking
    std::string lhsType = getExpressionType(leftNode);
    if (lhsType.empty()) return; // Return early if left-hand type couldn't be determined
    
    std::string rhsType = getExpressionType(rightNode);
    if (rhsType.empty()) return; // Return early if right-hand type couldn't be determined
    
    if (!areTypesCompatible(lhsType, rhsType)) {
        std::string leftName = getExpressionName(leftNode);
        std::string rightName = getExpressionName(rightNode);
        
        addError("'" + leftName + "' and expression '" + rightName + 
                "' are of different types", node->lineno);
    }
}

void SemanticAnalyzer::checkArrayAccess(Node* node) {
    if (!node || node->type != "ArrayAccess" || node->children.size() < 2) return;
    
    auto arrayExpr = node->children.front();
    auto indexExpr = *(++node->children.begin());
    
    std::string arrayType = getExpressionType(arrayExpr);
    if (!arrayType.empty() && !isArrayType(arrayType)) {
        addError("array access on non-array type '" + arrayType + "'", node->lineno);
    }
    
    std::string indexType = getExpressionType(indexExpr);
    if (!indexType.empty() && !isIntegerType(indexType)) {
        // Use exactly the same message format as in getExpressionType
        addError("array index must be an integer, got '" + indexType + "'", indexExpr->lineno);
    }
}

void SemanticAnalyzer::checkReturnType(Node* node) {
    if (!node || node->type != "Return") return;

    // Get the current method's key
    std::string key = currentClass + "." + currentMethod;
    
  
    // Check if this method was already reported as a duplicate
    if (isDuplicateMethod(currentClass, currentMethod)) {
        return;
    }
    
    // Get the expected return type from the current method
    auto classRecord = symbolTable.getClass(currentClass);
    if (!classRecord) return;
    
    auto methodIt = classRecord->methods.find(currentMethod);
    if (methodIt == classRecord->methods.end()) return;
    
    auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
    if (!methodRecord) return;
    
    std::string expectedType = methodRecord->returnType;
    
    // Get the method declaration line number instead of using the return statement line
    int methodLineNo = methodLines[key]; // Get line where method was declared
    
    // If return has no expression (void return)
    if (node->children.empty()) {
        if (expectedType != "void") {
            addError("invalid return type", methodLineNo); // Use method line number
        }
        return;
    }
    
    // Get the actual return expression type
    auto returnExpr = node->children.front();
    std::string actualType = getExpressionType(returnExpr);

    // Check if types are compatible
    if (!areTypesCompatible(expectedType, actualType)) {
        addError("invalid return type", methodLineNo); // Use method line number
    }
}

void SemanticAnalyzer::checkConditions(Node* node) {
    if (!node) return;
    
    if (node->type == "If" || node->type == "While") {
        if (node->children.empty()) return;
        
        auto condExpr = node->children.front();
        std::string condType = getExpressionType(condExpr);
        
        if (!condType.empty() && !isBooleanType(condType)) {
            addError("condition must be boolean, but got '" + condType + "'", condExpr->lineno);
        }
    }
}

void SemanticAnalyzer::addError(const std::string& message, int lineNo) {

    std::string error = "@error at line " + std::to_string(lineNo) + ". " + message;
    
    // Only add if this exact error isn't already in the list
    if (std::find(errors.begin(), errors.end(), error) == errors.end()) {
        errors.push_back(error);
    }
}

void SemanticAnalyzer::collectDeclarations(Node* node) {
    if (!node) return;
    
    if (node->type == "VarDeclaration") {
        // Process the declaration first to register it in the symbol table
        checkLocalVarDeclaration(node);
    } else {
        // Process all children to find declarations
        for (auto child : node->children) {
            collectDeclarations(child);
        }
    }
}

void SemanticAnalyzer::checkArrayAssignment(Node* node) {
    if (!node || node->type != "ArrayAssignment" || node->children.size() < 2) return;
    
    auto arrayExpr = node->children.front();
    auto indexExpr = *(++node->children.begin());
    auto valueExpr = *(++(++node->children.begin()));
    
    std::string arrayType = getExpressionType(arrayExpr);
    if (!arrayType.empty() && !isArrayType(arrayType)) {
        addError("array assignment on non-array type '" + arrayType + "'", node->lineno);
    }
    
    std::string indexType = getExpressionType(indexExpr);
    std::cout << "DEBUG: Array index type is '" << indexType << "'" << std::endl;

    if (!indexType.empty() && !isIntegerType(indexType)) {
        addError("array index must be an integer, but got '" + indexType + "'", indexExpr->lineno);
    }
    
    std::string valueType = getExpressionType(valueExpr);
    if (!valueType.empty() && !areTypesCompatible("int", valueType)) {
        addError("array assignment value must be an integer, but got '" + valueType + "'", valueExpr->lineno);
    }
}


bool SemanticAnalyzer::isDuplicateMethod(const std::string& className, const std::string& methodName) const {
    // Check if we've already reported this method as a duplicate
    std::string key = className + "." + methodName;
    
    // Count how many times this method appears in our error list
    for (const auto& error : errors) {
        if (error.find("Already Declared Function: '" + methodName + "'") != std::string::npos) {
            return true;
        }
    }
    return false;
}