#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "Node.h"
#include "SymbolTable.h"
#include "IR.h"

class IRGenerator {
private:
    CFG cfg;
    SymbolTable& symbolTable;
    BasicBlock* currentBlock;
    
    std::string currentClass;
    std::string currentMethod;
    
    // New traversal method for correct AST handling
    void traverseAST(Node* node);
    void processMethodDeclaration(Node* node);
    
    // Helper methods for expression processing
    std::string processExpression(Node* node);
    void processStatement(Node* node);
    void processMethodBody(Node* node);
    
    // Methods to handle specific node types
    std::string processIntegerLiteral(Node* node);
    std::string processBooleanLiteral(Node* node);
    std::string processIdentifier(Node* node);
    std::string processBinaryOp(Node* node);
    std::string processUnaryOp(Node* node);
    std::string processArrayAccess(Node* node);
    std::string processArrayLength(Node* node);
    std::string processMethodCall(Node* node);
    std::string processNewObject(Node* node);
    std::string processNewArray(Node* node);
    std::string processThisExpression(Node* node);
    
    // Methods to handle statement types
    void processIfStatement(Node* node);
    void processWhileStatement(Node* node);
    void processAssignment(Node* node);
    void processArrayAssignment(Node* node);
    void processReturn(Node* node);
    void processBlock(Node* node);
    void processPrintStatement(Node* node);

public:
    IRGenerator(SymbolTable& st) : symbolTable(st), currentBlock(nullptr) {}
    
    CFG generate(Node* root);
};

#endif // IR_GENERATOR_H