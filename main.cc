#include <iostream>
#include "parser.tab.hh"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "IRGenerator.h"  
#include "BytecodeGenerator.h"

extern Node *root;
extern FILE *yyin;
extern int yylineno;
extern int lexical_errors;
extern yy::parser::symbol_type yylex();

enum errCodes
{
    SUCCESS = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    AST_ERROR = 3,
    SEMANTIC_ERROR = 4,
    SEGMENTATION_FAULT = 139
};

int errCode = errCodes::SUCCESS;

// Handling Syntax Errors
void yy::parser::error(std::string const &err)
{
    if (!lexical_errors)
    {
        std::cerr << "Syntax errors found! See the logs below:" << std::endl;
        std::cerr << "\t@error at line " << yylineno << ". Cannot generate a syntax for this input:" << err.c_str() << std::endl;
        std::cerr << "End of syntax errors!" << std::endl;
        errCode = errCodes::SYNTAX_ERROR;
    }
}

void debugPrintAST(Node* node, int depth = 0) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    
    // Print node info
    std::cout << node->type << ": " << node->value << " (line: " << node->lineno << ")" << std::endl;
    

}

void printNodeTypeHierarchy(Node* node, int depth = 0) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    
    // Print node info
    std::cout << node->type << std::endl;
    
    // Recursively print children
    for (auto child : node->children) {
        printNodeTypeHierarchy(child, depth + 1);
    }
}

int main(int argc, char **argv)
{
    // Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }
    //
    if (USE_LEX_ONLY)
        yylex();
    else
    {
        yy::parser parser;

        bool parseSuccess = !parser.parse();

        if (lexical_errors)
            errCode = errCodes::LEXICAL_ERROR;

        if (parseSuccess && !lexical_errors) {
            try {
				                // Generate parse tree visualization
                if (root)
                {
                    root->generate_tree();
                }
                // Build symbol table first without reporting errors
                SymbolTable symbolTable;
                symbolTable.buildFromAST(root);
                
                // Generate symbol table visualization (optional)
                symbolTable.generateDotFile("symboltable.dot");
                
                // Run semantic analysis with two-pass approach
                SemanticAnalyzer semanticAnalyzer(symbolTable);
                semanticAnalyzer.analyze(root);
                
                // Check for semantic errors
                const auto& errors = semanticAnalyzer.getErrors();
                if (!errors.empty()) {
                    std::cerr << "Semantic errors found!" << std::endl;
                    for (const auto& error : errors) {
                        std::cerr << error << std::endl;
                    }
                    errCode = errCodes::SEMANTIC_ERROR;
                } else {
                    // No semantic errors, proceed to IR generation
                    std::cout << "Generating intermediate representation..." << std::endl;
                    IRGenerator irGenerator(symbolTable);
                    CFG cfg = irGenerator.generate(root);
                    
                    // Debug info about the CFG
                    std::cout << "CFG has " << cfg.blocks.size() << " blocks." << std::endl;
                    if (cfg.blocks.empty()) {
                        std::cerr << "Warning: CFG is empty! Check AST structure." << std::endl;
                    }

                    // Generate CFG visualization
                    cfg.generateDotFile("cfg.dot");
                    std::cout << "Generated control flow graph in cfg.dot" << std::endl;

                    // Generate bytecode
                    std::cout << "Generating bytecode..." << std::endl;
                    BytecodeGenerator bytecodeGenerator;
                    bytecodeGenerator.generateBytecode(cfg, "output.bc");
                    std::cout << "Bytecode generation complete." << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error during semantic analysis: " << e.what() << std::endl;
                errCode = errCodes::AST_ERROR;
            }
        }
    }

    return errCode;
}