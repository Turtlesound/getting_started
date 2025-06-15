#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Node.h"

// Forward declarations
class Record;
class ClassRecord;
class MethodRecord;
class VarRecord;

// Record class - base class for all symbol table entries
class Record {
public:
    std::string name;
    std::string type;
    Record(const std::string& n, const std::string& t);
    virtual std::string getKind() const = 0;
    virtual ~Record() = default;
};

// Class record
class ClassRecord : public Record {
public:
    std::map<std::string, std::shared_ptr<Record>> fields;
    std::map<std::string, std::shared_ptr<Record>> methods;
    
    ClassRecord(const std::string& name);
    std::string getKind() const override;
    std::vector<std::pair<std::string, std::string>> getAllFields() const;
    std::vector<std::pair<std::string, std::string>> getAllMethods() const;
};

// Method record
class MethodRecord : public Record {
public:
    // Add this vector to track parameter order
    std::vector<std::string> paramOrder;
    std::string returnType;
    std::map<std::string, std::shared_ptr<Record>> params;
    std::map<std::string, std::shared_ptr<Record>> locals;
    
    MethodRecord(const std::string& name, const std::string& retType);
    std::string getKind() const override;
};

// Variable record
class VarRecord : public Record {
public:
    bool isParam;
    
    VarRecord(const std::string& name, const std::string& type, bool param = false);
    std::string getKind() const override;
};

using FieldInfo = std::pair<std::string, std::string>;
using MethodInfo = std::pair<std::string, std::string>;

// Symbol table class
class SymbolTable {
private:
    std::map<std::string, std::shared_ptr<ClassRecord>> classes;
    std::string currentClass;
    std::string currentMethod;
    
    void processNode(Node* node);
    void addClass(const std::string& name);
    void addMethod(const std::string& name, const std::string& returnType);
    void addVariable(const std::string& name, const std::string& type, bool isParam = false);
    
public:
    SymbolTable() = default;
    
    // Build the symbol table from AST
    void buildFromAST(Node* root);
    
    // Reset the symbol table
    void reset();
    
    // Lookup symbols
    std::shared_ptr<Record> lookup(const std::string& name) const;
    std::shared_ptr<ClassRecord> getClass(const std::string& className) const;
    
    // Check if class exists
    bool isClassDeclared(const std::string& className) const;
    
    // Check for duplicates - moved to public for SemanticAnalyzer
    bool isClassDuplicate(const std::string& name) const;
    bool isDuplicateField(const std::string& className, const std::string& fieldName) const;
    bool isDuplicateMethod(const std::string& className, const std::string& methodName) const;
    bool isDuplicateParam(const std::string& className, const std::string& methodName, 
                         const std::string& paramName) const;
    bool isDuplicateLocal(const std::string& className, const std::string& methodName,
                         const std::string& localName) const;
                         
    // Helper methods for debugging/visualization
    void generateDotFile(const std::string& filename = "symboltable.dot") const;
    void printSymbolTable() const;
};

#endif // SYMBOL_TABLE_H