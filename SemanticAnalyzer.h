#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Node.h"
#include "SymbolTable.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_map>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(const SymbolTable& st);
    void analyze(Node* node);
    const std::vector<std::string>& getErrors() const;
    
private:
    const SymbolTable& symbolTable;
    std::vector<std::string> errors;
    std::string currentClass;
    std::string currentMethod;
    
    // Maps to track declarations
    std::unordered_map<std::string, int> classLines; // className -> line number
    std::unordered_map<std::string, int> methodLines; // className.methodName -> line number
    std::unordered_map<std::string, int> fieldLines; // className.fieldName -> line number
    std::unordered_map<std::string, int> paramLines; // className.methodName.paramName -> line number
    std::unordered_map<std::string, int> localLines; // className.methodName.localName -> line number
    
    // Set to track processed nodes to avoid duplicates
    std::set<Node*> processedNodes;
    
    // Helper methods for type checking
    bool isIntegerType(const std::string& type) const;
    bool isBooleanType(const std::string& type) const;
    bool isArrayType(const std::string& type) const;
    bool isClassType(const std::string& type) const;
    bool areTypesCompatible(const std::string& expectedType, const std::string& actualType) const;
    std::string getExpressionType(Node* node);
    
    // Semantic checks
    void checkClassDeclaration(Node* node);
    void checkFieldDeclaration(Node* node);
    void checkMethodDeclaration(Node* node);
    void checkMethodCall(Node* node);
    void checkAssignment(Node* node);
    void checkArrayAccess(Node* node);
    void checkReturnType(Node* node);
    void checkConditions(Node* node);
    void checkLocalVarDeclaration(Node* node);
    void checkParameterDeclaration(Node* node);
    std::string getExpressionName(Node* node);
    void collectDeclarations(Node* node);
    void checkArrayAssignment(Node* node);
    
    // Error reporting
    void addError(const std::string& message, int lineNo);
};

#endif // SEMANTIC_ANALYZER_H