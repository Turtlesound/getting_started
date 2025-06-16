#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "Node.h"
#include "SymbolTable.h"
#include "IR.h"
#include <map>
#include <string>

class IRGenerator {
public:
    IRGenerator() : currentBlock(nullptr), tempCounter(0) {}
    IRGenerator(SymbolTable& st) : symbolTable(st), currentBlock(nullptr), tempCounter(0) {}
    CFG generate(Node* root);

private:
    SymbolTable symbolTable;
    std::string currentClass;
    BasicBlock* currentBlock;
    int tempCounter;
    CFG cfg;
    
    // Map of method names to their entry blocks
    std::map<std::string, BasicBlock*> methodEntryPoints;
    
    
    std::string getMethodFullName(const std::string& className, const std::string& methodName);
    void collectMethodEntries(Node* node);
    void generateMethodBodies(Node* node);
    
    void traverseAST(Node* node);
    std::vector<Node*> findNodesOfType(Node* root, const std::string& type);
    void processMethodDeclaration(Node* node);
    std::string processExpression(Node* node);
    void processStatement(Node* node);
    void processIfStatement(Node* node);
    void processWhileStatement(Node* node);
    void processMethodBody(Node* node);
    void processBlock(Node* node);
    void processPrintStatement(Node* node);
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
    void processAssignment(Node* node);
    void processArrayAssignment(Node* node);
    void processReturn(Node* node);
    
    std::string newTemp() {
        return "t" + std::to_string(tempCounter++);
    }

    std::string currentMethod; 
};

#endif // IR_GENERATOR_H