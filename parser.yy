/* Skeleton and definitions for generating a LALR(1) parser in C++ */
%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

/* Required code included before the parser definition begins */
%code requires{
  #include <string>
  #include "Node.h"
  #include <vector>
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}


/* Code included in the parser implementation file */
%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
  // Add these variables to track line numbers of specific tokens
  int class_line = 0;
  int method_line = 0;
}

/* Token definitions for the grammar */
/* Tokens represent the smallest units of the language, like operators and parentheses */
%token <std::string> PLUSOP SUBOP MULTOP DIVOP LP RP LESSOP GREATOP AND OR MOD EQUAL LS RS 
%token <std::string> TRUE FALSE DOT COMMA THIS NEW NOT PUBLIC CLASS STATIC VOID MAIN STRING 
%token <std::string> BOOLEAN IF ELSE WHILE SYSTEMOUTPRINTLN RETURN LENGTH LBRACE RBRACE 
%token <std::string> SEMICOLON ID INT INTLITERAL ASSIGN
%token END 0 "end of file"


/* Operator precedence and associativity rules, from lowest to highest precedence */
%left OR
%left AND
%left EQUAL
%right ASSIGN  /* Change from %left to %right for assignment */
%left LESSOP GREATOP
%left PLUSOP SUBOP
%left MULTOP DIVOP MOD
%right NOT
%left DOT
%precedence ID  /* Add ID precedence here */
%left LS RS     /* Keep array access operators together */
%left LP RP

/* Specify types for non-terminals in the grammar */
/* The type specifies the data type of the values associated with these non-terminals */
%type <Node*> root statement expression expression_list type statement_list method_declaration var_declaration class_declaration  var_declaration_list method_declaration_list class_declaration_list main_class goal param_list non_empty_expression_list compound_statement simple_statement method_body non_return_statement_list return_statement base_type array_type type_identifier

/* Grammar rules section */
/* This section defines the production rules for the language being parsed */

%%

root:       goal END { root = $1; }


goal: main_class class_declaration_list { 
                $$= new Node("Goal", "", yylineno);
                $$->children.push_back($1);
                $$->children.push_back($2);
            }

main_class: PUBLIC 
    { 
        method_line = yylineno; // Store the line number when PUBLIC token is seen 
    }
    CLASS ID LBRACE PUBLIC STATIC VOID MAIN LP STRING LS RS ID 
    RP LBRACE statement statement_list RBRACE RBRACE { 
        $$ = new Node("MainClass", "", method_line); // Use stored line number
        $$->children.push_back(new Node("Identifier", $4, method_line));
        $$->children.push_back($17);
    };

class_declaration_list: class_declaration_list class_declaration
     { $$ = $1;
     $$->children.push_back($2); }
    | /* empty */ { $$ = new Node("ClassDeclarationList", "", yylineno); };

class_declaration: CLASS 
    { 
        class_line = yylineno; // Store the line number when CLASS token is seen 
    }
    ID LBRACE var_declaration_list method_declaration_list RBRACE 
    { 
        $$ = new Node("ClassDeclaration", "", class_line); // Use stored line number
        $$->children.push_back(new Node("Identifier", $3, class_line));
        $$->children.push_back($5);
        $$->children.push_back($6);
    };

var_declaration_list: var_declaration_list var_declaration 
    { $$ = $1; $$->children.push_back($2); }
    | /* empty */ { $$ = new Node("VarDeclarationList", "", yylineno); };

var_declaration: 
    type ID SEMICOLON {
        $$ = new Node("VarDeclaration", "", yylineno);
        $$->children.push_back($1);
        $$->children.push_back(new Node("Identifier", $2, yylineno));
    }
    | type LS RS ID SEMICOLON { 
  $$ = new Node("VarDeclaration", "", yylineno);
  $$->children.push_back(new Node("TypeArray", $1->value, yylineno));
  $$->children.push_back(new Node("Identifier", $4, yylineno));
};

type: base_type { $$ = $1; }
    | array_type { $$ = $1; }
    | BOOLEAN { $$ = new Node("Boolean", "boolean", yylineno); }
    | INT { $$ = new Node("Int", "int", yylineno); }
    ;

base_type: 
    STRING { $$ = new Node("String", "string", yylineno); }
    | VOID { $$ = new Node("Void", "void", yylineno); }
    | type_identifier { $$ = $1; }
    ;

/* Modify the type_identifier and expression rules */
type_identifier:
    ID  %prec ID 
    { 
        $$ = new Node("UserDefinedType", $1, yylineno); 
    }
    ;

array_type:
    INT LS RS { $$ = new Node("IntArray", "int[]", yylineno); }
    | base_type LS RS { 
        $$ = new Node("ArrayType", $1->value + "[]", yylineno);
        $$->children.push_back($1);
    }
    ;

method_declaration_list: method_declaration_list method_declaration { $$ = $1; $$->children.push_back($2); }
                        | /* empty */ { $$ = new Node("MethodDeclarationList", "", yylineno); };

method_declaration: PUBLIC 
    { 
        method_line = yylineno; // Store the line number when PUBLIC token is seen 
    }
    type ID LP param_list RP LBRACE method_body RBRACE 
    { 
        $$ = new Node("MethodDeclaration", "", method_line); // Use stored line number
        $$->children.push_back(new Node("Public", "public", method_line));
        $$->children.push_back($3);
        $$->children.push_back(new Node("Identifier", $4, method_line));
        $$->children.push_back($6);
        $$->children.push_back($9);
    };

method_body: non_return_statement_list return_statement
    {
        $$ = new Node("MethodBody", "", yylineno);
        $$->children.push_back($1);
        $$->children.push_back($2);
    }
    | return_statement
    {
        $$ = new Node("MethodBody", "", yylineno);
        $$->children.push_back($1);
    };

return_statement: RETURN expression SEMICOLON
    {
        $$ = new Node("ReturnStatement", "", yylineno);
        $$->children.push_back($2);
    };

non_return_statement_list: non_return_statement_list statement
    {
        $$ = $1;
        $$->children.push_back($2);
    }
    | /* empty */
    {
        $$ = new Node("StatementList", "", yylineno);
    };

param_list: type ID { 
        $$ = new Node("ParameterList", "", yylineno);
        $$->children.push_back(new Node("Type", $1->value, yylineno));
        $$->children.push_back(new Node("Identifier", $2, yylineno));
    }
    | param_list COMMA type ID { 
        $$ = $1;
        $$->children.push_back(new Node("Type", $3->value, yylineno));
        $$->children.push_back(new Node("Identifier", $4, yylineno));
    }
    | /* empty */ { $$ = new Node("ParameterList", "", yylineno); };


statement_list: statement_list statement     { $$ = $1; $$->children.push_back($2); }
    | /* empty */                           { $$ = new Node("StatementList", "", yylineno); }
    ;

statement: simple_statement                  { $$ = $1; }
    | compound_statement                     { $$ = $1; }
    | var_declaration                        { $$ = $1; }
    ;

simple_statement: 
    ID ASSIGN expression SEMICOLON        
    { 
        $$ = new Node("Assignment", "", yylineno);
        $$->children.push_back(new Node("Identifier", $1, yylineno));
        $$->children.push_back($3);
    }
    | expression SEMICOLON                  
    {
        $$ = new Node("ExpressionStatement", "", yylineno);
        $$->children.push_back($1);
    }
    | SYSTEMOUTPRINTLN LP expression RP SEMICOLON {
        $$ = new Node("PrintStatement", "", yylineno);
        $$->children.push_back($3);
    }
    | ID LS expression RS ASSIGN expression SEMICOLON {
        $$ = new Node("ArrayAssignment", "", yylineno);
        $$->children.push_back(new Node("Identifier", $1, yylineno));
        $$->children.push_back($3);
        $$->children.push_back($6);
    }
    ;

compound_statement:
    IF LP expression RP statement           { 
        $$ = new Node("IfStatement", "", yylineno);
        $$->children.push_back($3);
        $$->children.push_back($5);
    }
    | IF LP expression RP statement ELSE statement {
        $$ = new Node("IfElseStatement", "", yylineno);
        $$->children.push_back($3);
        $$->children.push_back($5);
        $$->children.push_back($7);
    }
    | WHILE LP expression RP statement      {
        $$ = new Node("WhileStatement", "", yylineno);
        $$->children.push_back($3);
        $$->children.push_back($5);
    }
    | LBRACE statement_list RBRACE         {
        $$ = new Node("Block", "", yylineno);
        $$->children.push_back($2);
    }
    ;

expression_list:
    non_empty_expression_list
    | /* empty */ { $$ = new Node("ExpressionList", "", yylineno); };

non_empty_expression_list:
    expression { $$ = new Node("ExpressionList", "", yylineno); $$->children.push_back($1); }
    | non_empty_expression_list COMMA expression { $$ = $1; $$->children.push_back($3); };


expression: expression PLUSOP expression {      /*
                                                  Create a subtree that corresponds to the AddExpression
                                                  The root of the subtree is AddExpression
                                                  The childdren of the AddExpression subtree are the left hand side (expression accessed through $1) and right hand side of the expression (expression accessed through $3)
                                                */
                            $$ = new Node("AddExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r1 "); */
                          }
            | expression SUBOP expression {
                            $$ = new Node("SubExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r2 "); */
                          }
            | expression MULTOP expression {
                            $$ = new Node("MultExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r3 "); */
                          }
            | expression DIVOP expression {
                            $$ = new Node("DivExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }             
            | expression LESSOP expression {
                            $$ = new Node("LessThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r6 "); */
                          }
            | expression GREATOP expression {
                            $$ = new Node("GreaterThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r7 "); */
                          }
            | expression AND expression{
                          $$ = new Node ("AndExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                          /* printf("r7"); */
                        }
            | expression OR expression{
                          $$ = new Node ("OrExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                          /* printf("r8"); */
                        }
            | expression EQUAL expression{
                          $$ = new Node ("EqualExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                          /* printf("r9"); */
                        }
            | NOT expression{
                          $$ = new Node ("NotExpression", "", yylineno);
                          $$->children.push_back($2);
                          /* printf("r10"); */
                        }
            | expression MOD expression{
                          $$ = new Node ("ModExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                          /* printf("r11"); */
                        }
            | NEW type LS expression RS { $$ = new Node("NewArray", "", yylineno);
                                        $$->children.push_back($4);
                                      }
            | expression DOT LENGTH { $$ = new Node("LengthExpression", "", yylineno);
                                        $$->children.push_back($1);
                                    }
            | expression DOT ID LP expression_list RP { $$ = new Node("MethodCall", "", yylineno);
                                                            $$->children.push_back($1);
                                                            $$->children.push_back(new Node("Identifier", $3, yylineno));
                                                            $$->children.push_back($5);
                                                        }
            | expression LS expression RS
                        {
                            $$ = new Node("ArrayAccess", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                        }
            | ID LS expression RS %prec LS
                        {
                            $$ = new Node("ArrayAccess", "", yylineno);
                            $$->children.push_back(new Node("Identifier", $1, yylineno));
                            $$->children.push_back($3);
                        }
            | LP expression RP { $$ = $2; /* printf("r6 ");  simply return the expression */}
            | ID %prec ID           
            { 
                $$ = new Node("Identifier", $1, yylineno); 
            }
            | TRUE         { $$ = new Node("True", "", yylineno); /* printf("r8 "); */}
            | FALSE        { $$ = new Node("False", "", yylineno); /* printf("r9 "); */}
            | THIS         { $$ = new Node("This", "", yylineno); /* printf("r10 "); */}
            | INTLITERAL { $$ = new Node("IntegerLiteral", $1, yylineno); /* printf("r12 "); */}
            | NEW ID LP RP { $$ = new Node("NewObject", "", yylineno);
                            $$->children.push_back(new Node("Identifier", $2, yylineno));
                          }
            | INT LS expression RS { $$ = new Node("IntArray", "", yylineno);
                                    $$->children.push_back($3);
                                  }
            | NEW INT LS expression RS {
                $$ = new Node("NewIntArray", "", yylineno);
                $$->children.push_back(new Node("New", "new", yylineno));
                $$->children.push_back(new Node("Type", "int", yylineno));
                $$->children.push_back($4); // Storleken på arrayen
            }
    ;
