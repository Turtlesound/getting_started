#include "SymbolTable.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// Record implementations
Record::Record(const std::string& n, const std::string& t) : name(n), type(t) {}

// ClassRecord implementations
ClassRecord::ClassRecord(const std::string& name) : Record(name, name) {}

std::string ClassRecord::getKind() const {
    return "Class";
}

std::vector<FieldInfo> ClassRecord::getAllFields() const {
    std::vector<FieldInfo> result;
    for (const auto& pair : fields) {
        auto field = std::dynamic_pointer_cast<VarRecord>(pair.second);
        if (field) {
            result.push_back({field->name, field->type});
        }
    }
    return result;
}

std::vector<MethodInfo> ClassRecord::getAllMethods() const {
    std::vector<MethodInfo> result;
    for (const auto& pair : methods) {
        auto method = std::dynamic_pointer_cast<MethodRecord>(pair.second);
        if (method) {
            result.push_back({method->name, method->returnType});
        }
    }
    return result;
}

// MethodRecord implementations
MethodRecord::MethodRecord(const std::string& name, const std::string& retType) 
    : Record(name, retType), returnType(retType) {}

std::string MethodRecord::getKind() const {
    return "Method";
}

// VarRecord implementations
VarRecord::VarRecord(const std::string& name, const std::string& type, bool param) 
    : Record(name, type), isParam(param) {}

std::string VarRecord::getKind() const {
    return isParam ? "Parameter" : "Variable";
}

// SymbolTable implementations
void SymbolTable::reset() {
    classes.clear();
    currentClass.clear();
    currentMethod.clear();
}

void SymbolTable::buildFromAST(Node* root) {
    reset();
    processNode(root);
}

void SymbolTable::processNode(Node* node) {
    if (!node) return;
    
    if (node->type == "ClassDeclaration" && !node->children.empty()) {
        // Get class name
        std::string className = node->children.front()->value;
        
        // Add class to symbol table
        addClass(className);
        currentClass = className;
        
        // Process class body
        for (auto child : node->children) {
            processNode(child);
        }
        
        currentClass = "";
    }
    else if (node->type == "VarDeclaration" && node->children.size() >= 2) {
        // Handle variable declarations (including class fields)
        std::string varType = node->children.front()->value;
        std::string varName = node->children.back()->value;
        
        // Add variable to symbol table
        // When currentMethod is empty, it will be added as a class field
        addVariable(varName, varType, false);
    }
    else if (node->type == "MethodDeclaration" && !node->children.empty()) {
        auto it = node->children.begin();
        if ((*it)->type == "Public") ++it;
        
        std::string returnType = (*it)->value;
        ++it;
        
        std::string methodName = (*it)->value;
        
        // Add method to symbol table
        addMethod(methodName, returnType);
        currentMethod = methodName;
        
        // Process method parameters
        ++it;
        if (it != node->children.end() && (*it)->type == "ParameterList") {
            std::map<std::string, bool> seenParams;
            for (auto paramNode : (*it)->children) {
                if (paramNode->type == "Parameter") {
                    std::string paramType = paramNode->children.front()->value;
                    std::string paramName = paramNode->children.back()->value;

                    seenParams[paramName] = true;
                    // Add parameter to symbol table
                    addVariable(paramName, paramType, true);
                }
            }
        }
        
        // Process method body
        for (auto child : node->children) {
            processNode(child);
        }
        
        // DEBUG: Print MethodDeclaration children types
        std::cout << "DEBUG: MethodDeclaration children: ";
        for (auto child : node->children) {
            std::cout << child->type << " ";
        }
        std::cout << std::endl;
        
        currentMethod = "";
    }
    else {
        // Process other nodes
        for (auto child : node->children) {
            processNode(child);
        }
    }
}

void SymbolTable::addClass(const std::string& name) {
    classes[name] = std::make_shared<ClassRecord>(name);
}

void SymbolTable::addMethod(const std::string& name, const std::string& returnType) {
    if (currentClass.empty() || classes.find(currentClass) == classes.end()) {
        return;
    }
    // Prevent duplicate method registration
    if (classes[currentClass]->methods.find(name) != classes[currentClass]->methods.end()) {
        return; // Do not add duplicate method
    }
    auto method = std::make_shared<MethodRecord>(name, returnType);
    classes[currentClass]->methods[name] = method;
}

void SymbolTable::addVariable(const std::string& name, const std::string& type, bool isParam) {
    // Debug the variable addition
    std::cout << "DEBUG: Adding variable: " << name << " of type: " << type 
              << " isParam: " << (isParam ? "true" : "false") << std::endl;
    std::cout << "DEBUG: Current class: " << currentClass << ", current method: " << currentMethod << std::endl;
    
    if (currentClass.empty()) {
        std::cerr << "Error: Cannot add variable outside a class" << std::endl;
        return;
    }
    auto classRecord = getClass(currentClass);
    if (!classRecord) {
        std::cerr << "Error: Class " << currentClass << " not found" << std::endl;
        return;
    }
    auto varRecord = std::make_shared<VarRecord>(name, type, isParam);
    if (currentMethod.empty()) {
        // Class field
        if (classRecord->fields.find(name) != classRecord->fields.end()) {
            return; // Do not add duplicate field
        }
        classRecord->fields[name] = varRecord;
    } else {
        auto methodIt = classRecord->methods.find(currentMethod);
        if (methodIt == classRecord->methods.end()) {
            std::cerr << "Error: Method " << currentMethod << " not found in class " << currentClass << std::endl;
            return;
        }
        auto methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
        if (!methodRecord) {
            std::cerr << "Error: Failed to cast to MethodRecord" << std::endl;
            return;
        }
        if (isParam) {
            // Always add the parameter name to paramOrder to reflect the declared arity.
            // The semantic analyzer will handle reporting duplicate parameter names separately.
            methodRecord->paramOrder.push_back(name);

            if (methodRecord->params.find(name) != methodRecord->params.end()) {
                // This is a duplicate parameter name.
                // The semantic analyzer (e.g., checkParameterDeclaration) is responsible for reporting this error.
                // We've already added it to paramOrder for arity checking.
                // We don't add it to the 'params' map again, as the map stores unique names (the first one wins).
                return; 
            }
            methodRecord->params[name] = varRecord;
        } else {
            if (methodRecord->locals.find(name) != methodRecord->locals.end()) {
                return; // Do not add duplicate local
            }
            methodRecord->locals[name] = varRecord;
        }
    }
}

std::shared_ptr<Record> SymbolTable::lookup(const std::string& name) const {
    // Method local or parameter
    if (!currentMethod.empty() && !currentClass.empty()) {
        auto classIt = classes.find(currentClass);
        if (classIt != classes.end()) {
            auto methodIt = classIt->second->methods.find(currentMethod);
            if (methodIt != classIt->second->methods.end()) {
                auto method = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
                
                // Check parameters
                auto paramIt = method->params.find(name);
                if (paramIt != method->params.end()) {
                    return paramIt->second;
                }
                
                // Check locals
                auto localIt = method->locals.find(name);
                if (localIt != method->locals.end()) {
                    return localIt->second;
                }
            }
        }
    }
    
    // Class field
    if (!currentClass.empty()) {
        auto classIt = classes.find(currentClass);
        if (classIt != classes.end()) {
            auto fieldIt = classIt->second->fields.find(name);
            if (fieldIt != classIt->second->fields.end()) {
                return fieldIt->second;
            }
        }
    }
    
    // Class
    auto classIt = classes.find(name);
    if (classIt != classes.end()) {
        return classIt->second;
    }
    
    // Method
    if (!currentClass.empty()) {
        auto classIt = classes.find(currentClass);
        if (classIt != classes.end()) {
            auto methodIt = classIt->second->methods.find(name);
            if (methodIt != classIt->second->methods.end()) {
                return methodIt->second;
            }
        }
    }
    
    return nullptr;
}

std::shared_ptr<ClassRecord> SymbolTable::getClass(const std::string& className) const {
    auto it = classes.find(className);
    return (it != classes.end()) ? it->second : nullptr;
}

bool SymbolTable::isClassDeclared(const std::string& className) const {
    return classes.find(className) != classes.end();
}

bool SymbolTable::isClassDuplicate(const std::string& name) const {
    return classes.find(name) != classes.end();
}

bool SymbolTable::isDuplicateField(const std::string& className, const std::string& fieldName) const {
    auto classIt = classes.find(className);
    if (classIt == classes.end()) {
        return false;
    }
    
    return classIt->second->fields.find(fieldName) != classIt->second->fields.end();
}

bool SymbolTable::isDuplicateMethod(const std::string& className, const std::string& methodName) const {
    auto classIt = classes.find(className);
    if (classIt == classes.end()) {
        return false;
    }
    
    return classIt->second->methods.find(methodName) != classIt->second->methods.end();
}

bool SymbolTable::isDuplicateParam(const std::string& className, const std::string& methodName, 
                                   const std::string& paramName) const {
    auto classIt = classes.find(className);
    if (classIt == classes.end()) {
        return false;
    }
    
    auto methodIt = classIt->second->methods.find(methodName);
    if (methodIt == classIt->second->methods.end()) {
        return false;
    }
    
    auto method = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
    if (!method) {
        return false;
    }
    
    return method->params.find(paramName) != method->params.end();
}

bool SymbolTable::isDuplicateLocal(const std::string& className, const std::string& methodName,
                                   const std::string& localName) const {
    auto classIt = classes.find(className);
    if (classIt == classes.end()) {
        return false;
    }
    
    auto methodIt = classIt->second->methods.find(methodName);
    if (methodIt == classIt->second->methods.end()) {
        return false;
    }
    
    auto method = std::dynamic_pointer_cast<MethodRecord>(methodIt->second);
    if (!method) {
        return false;
    }
    
    // Check if it's already a parameter
    if (method->params.find(localName) != method->params.end()) {
        return true;
    }
    
    // Check if it's already a local
    return method->locals.find(localName) != method->locals.end();
}

void SymbolTable::generateDotFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    
    outFile << "digraph SymbolTable {" << std::endl;
    outFile << "  node [shape=record];" << std::endl;
    
    // Generate nodes for classes
    for (const auto& classPair : classes) {
        const auto& className = classPair.first;
        const auto& classRecord = classPair.second;
        
        // Class node
        outFile << "  " << className << " [label=\"{Class: " << className << "|";
        
        // Fields
        outFile << "{Fields|";
        bool first = true;
        for (const auto& field : classRecord->getAllFields()) {
            if (!first) outFile << "\\l";
            outFile << field.first << ": " << field.second;
            first = false;
        }
        if (first) outFile << "none";
        outFile << "}|";
        
        // Methods
        outFile << "{Methods|";
        first = true;
        for (const auto& method : classRecord->getAllMethods()) {
            if (!first) outFile << "\\l";
            outFile << method.first << ": " << method.second;
            first = false;
        }
        if (first) outFile << "none";
        outFile << "}";
        
        outFile << "}\"];" << std::endl;
        
        // Generate nodes for methods
        for (const auto& methodPair : classRecord->methods) {
            const auto& methodName = methodPair.first;
            const auto& methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodPair.second);
            
            if (methodRecord) {
                std::string methodNodeName = className + "_" + methodName;
                
                // Method node
                outFile << "  " << methodNodeName << " [label=\"{Method: " << methodName << "|";
                outFile << "Return Type: " << methodRecord->returnType << "|";
                
                // Parameters
                outFile << "{Parameters|";
                first = true;
                for (const auto& paramPair : methodRecord->params) {
                    const auto& param = std::dynamic_pointer_cast<VarRecord>(paramPair.second);
                    if (param) {
                        if (!first) outFile << "\\l";
                        outFile << param->name << ": " << param->type;
                        first = false;
                    }
                }
                if (first) outFile << "none";
                outFile << "}|";
                
                // Locals
                outFile << "{Locals|";
                first = true;
                for (const auto& localPair : methodRecord->locals) {
                    const auto& local = std::dynamic_pointer_cast<VarRecord>(localPair.second);
                    if (local) {
                        if (!first) outFile << "\\l";
                        outFile << local->name << ": " << local->type;
                        first = false;
                    }
                }
                if (first) outFile << "none";
                outFile << "}";
                
                outFile << "}\"];" << std::endl;
                
                // Edge from class to method
                outFile << "  " << className << " -> " << methodNodeName << ";" << std::endl;
            }
        }
    }
    
    outFile << "}" << std::endl;
    outFile.close();
    
    std::cout << "Symbol table visualization generated at " << filename << std::endl;
}

void SymbolTable::printSymbolTable() const {
    std::cout << "Symbol Table:" << std::endl;
    
    for (const auto& classPair : classes) {
        const auto& className = classPair.first;
        const auto& classRecord = classPair.second;
        
        std::cout << "Class: " << className << std::endl;
        
        // Print fields
        std::cout << "  Fields:" << std::endl;
        for (const auto& field : classRecord->getAllFields()) {
            std::cout << "    " << field.first << ": " << field.second << std::endl;
        }
        
        // Print methods
        std::cout << "  Methods:" << std::endl;
        for (const auto& methodPair : classRecord->methods) {
            const auto& methodName = methodPair.first;
            const auto& methodRecord = std::dynamic_pointer_cast<MethodRecord>(methodPair.second);
            
            if (methodRecord) {
                std::cout << "    " << methodName << ": " << methodRecord->returnType << std::endl;
                
                // Print parameters
                std::cout << "      Parameters:" << std::endl;
                for (const auto& paramPair : methodRecord->params) {
                    const auto& param = std::dynamic_pointer_cast<VarRecord>(paramPair.second);
                    if (param) {
                        std::cout << "        " << param->name << ": " << param->type << std::endl;
                    }
                }
                
                // Print locals
                std::cout << "      Locals:" << std::endl;
                for (const auto& localPair : methodRecord->locals) {
                    const auto& local = std::dynamic_pointer_cast<VarRecord>(localPair.second);
                    if (local) {
                        std::cout << "        " << local->name << ": " << local->type << std::endl;
                    }
                }
            }
        }
    }
}

