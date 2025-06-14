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
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}

/* Code included in the parser implementation file */
%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
}

/* Token definitions for the grammar */
/* Tokens represent the smallest units of the language, like operators and parentheses */
%token <std::string> PLUSOP MINUSOP MULTOP INTLITERAL ID INT LP RP LESSOP GREATOP AND OR ASSIGN EQUAL LS RS TRUE FALSE DOT COMMA THIS NEW NOT PUBLIC CLASS STATIC VOID MAIN STRING BOOLEAN IF ELSE WHILE SYSTEMOUTPRINTLN RETURN LENGTH LBRACE RBRACE SEMICOLON DOLLAR EMPTY AT INVALID QUOTE MOD
%token END 0 "end of file"

/* Operator precedence and associativity rules */
/* Used to resolve ambiguities in parsing expressions See https://www.gnu.org/software/bison/manual/bison.html#Precedence-Decl */ 
%left OR
%left AND
%left EQUAL ASSIGN
%left LESSOP GREATOP
%left PLUSOP MINUSOP
%left MULTOP
%left NOT
%left DOT
%left LS RS

/* Specify types for non-terminals in the grammar */
/* The type specifies the data type of the values associated with these non-terminals */
%type <Node*> root expression expression_list statement statement_list Parameter MethodDeclarationBody 
%type <Node*> var_declaration  type MethodDeclarationParamsOpt MethodDeclarationParams method_declaration MethodDeclarationCL VarDeclarationCL
%type <Node*> mainclass class_declaration class_declaration_list  GOAL
%type <Node*> ElseStatementOpt Identifier

/* Grammar rules section */
/* This section defines the production rules for the language being parsed */
%%

root: GOAL { root = $1; };

GOAL: mainclass class_declaration_list END {
  $$ = new Node("Goal", "", yylineno);
  $$->children.push_back($1);
  $$->children.push_back($2);
};

mainclass: PUBLIC CLASS Identifier LBRACE 
          PUBLIC STATIC VOID MAIN LP STRING LS RS Identifier RP 
          LBRACE statement statement_list RBRACE 
          RBRACE {
    $$ = new Node("MainClass","", yylineno);
    $$->children.push_back($3);
    $$->children.push_back($16);
};

class_declaration_list: /* empty */ {
    $$ = new Node("ClassDeclarationList", "", yylineno);
}
| class_declaration_list class_declaration {
    $$ = $1;
    $$->children.push_back($2);
};

class_declaration: CLASS Identifier LBRACE 
                  VarDeclarationCL
                  MethodDeclarationCL
                  RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($4);
    $$->children.push_back($5);
};

VarDeclarationCL:
    { $$ = new Node("VarDeclarationCL", "", yylineno); }
    |var_declaration{
        $$ = new Node("VarDeclarationCL", "", yylineno);
        $$->children.push_back($1);
    }
    | VarDeclarationCL var_declaration{
        $$ = $1;
        $$->children.push_back($2);
    };

MethodDeclarationCL:
    { $$ = new Node("MethodDeclarationCL", "", yylineno); }
    |method_declaration{
        $$ = new Node("MethodDeclarationCL", "", yylineno);
        $$->children.push_back($1);
    }
    | MethodDeclarationCL method_declaration{
        $$ = $1;
        $$->children.push_back($2);
    };

MethodDeclarationBody:
    { $$ = new Node("MethodDeclarationBody", "", yylineno); }
    |var_declaration{
        $$ = new Node("MethodDeclarationBody", "", yylineno);
        $$->children.push_back($1);
    }
    | statement{
        $$ = new Node("MethodDeclarationBody", "", yylineno);
        $$->children.push_back($1);
    }
    | MethodDeclarationBody var_declaration{
        $$ = $1;
        $$->children.push_back($2);
    }
    | MethodDeclarationBody statement{
        $$ = $1;
        $$->children.push_back($2);
    };


var_declaration: type Identifier SEMICOLON {
    $$ = new Node("VarDeclaration", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($2);
};


method_declaration: PUBLIC type Identifier LP MethodDeclarationParamsOpt RP LBRACE 
                   MethodDeclarationBody
                   RETURN expression SEMICOLON 
                   RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);  // return type
    $$->children.push_back($3);  // method name
    $$->children.push_back($5);  // params
    $$->children.push_back($8);  // method body
    $$->children.push_back($10);  // return expression
} | PUBLIC type Identifier LP MethodDeclarationParamsOpt RP LBRACE statement RETURN expression SEMICOLON RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);  // return type
    $$->children.push_back($3);  // method name
    $$->children.push_back($5);  // params
    $$->children.push_back($8);  // method body
    $$->children.push_back($10);  // return expression
};

MethodDeclarationParamsOpt:
    { $$ = new Node("MethodDeclarationParamsOpt", "", yylineno); }
    |MethodDeclarationParams{
        $$ = new Node("MethodDeclarationParamsOpt", "", yylineno);
        $$->children.push_back($1);
    };

MethodDeclarationParams:
   Parameter { $$ = new Node("ParameterList", "", yylineno); $$->children.push_back($1); }
  | MethodDeclarationParams COMMA Parameter { 
          $$ = $1; 
          $$->children.push_back($3); 
      }
;


Parameter:
    type Identifier {
         $$ = new Node("Parameter", "", yylineno);
         $$->children.push_back($1);
         $$->children.push_back($2);
    }
    ;


type: INT { $$ = new Node("Type", "int", yylineno); }
    | BOOLEAN { $$ = new Node("Type", "boolean", yylineno); }
    | INT LS RS { $$ = new Node("Type", "int[]", yylineno); }
    | STRING { $$ = new Node("Type", "String", yylineno); }
    | VOID { $$ = new Node("Type", "void", yylineno); }
    | ID { $$ = new Node("Type", $1 , yylineno); };

statement_list: /* empty */ {
    $$ = nullptr;
} | statement statement_list {
    $$ = new Node("statement_list", "", yylineno);
    $$->children.push_back($1);   // Identifier Node
    if ($2 != nullptr) {          
        for (auto child : $2->children) {
            $$->children.push_back(child);
        }
    }
};

statement: LBRACE statement_list RBRACE {
    $$ = new Node("BlockStatement", "BlockStatement", yylineno);
    if($2 != nullptr) $$->children.push_back($2); // StatementList Node
    $$->children.push_back(new Node("BlockStatement_End", "", yylineno));
} | IF LP expression RP statement ElseStatementOpt {
    $$ = new Node("IfStatement", "", yylineno);
    $$->children.push_back($3); // Expression Node
    if ($5 != nullptr) $$->children.push_back($5); // Statement Node
    if ($6 != nullptr) $$->children.push_back($6); // ElseStatementOpt Node
} | WHILE LP expression RP statement {
    $$ = new Node("WhileStatement", "", yylineno);
    $$->children.push_back($3); // Expression Node
    if($5 != nullptr)$$->children.push_back($5); // Statement Node
} | SYSTEMOUTPRINTLN LP expression RP SEMICOLON {
    $$ = new Node("PrintStatement", "", yylineno);
    $$->children.push_back($3); // Expression Node
} | Identifier ASSIGN expression SEMICOLON {
    $$ = new Node("AssignmentStatement", "", yylineno);
    $$->children.push_back($1); // Identifier Node
    $$->children.push_back($3); // Expression Node
} | Identifier LS expression RS ASSIGN expression SEMICOLON {
    $$ = new Node("ArrayAssignmentStatement", "", yylineno);
    $$->children.push_back($1); // Identifier Node
    $$->children.push_back($3); // Index Expression Node
    $$->children.push_back($6); // Value Expression Node
};

ElseStatementOpt: /* empty */ {
    $$ = new Node("ElseStatementOpt", "", yylineno);
} | ELSE statement {
    $$ = new Node("ElseStatement", "", yylineno);
    $$->children.push_back($2); // Statement Node
};

expression_list: expression { $$ = new Node("ExpressionList", "", yylineno); $$->children.push_back($1); }
| expression_list expression { $$=$1; $$->children.push_back($2); }
| expression_list COMMA expression { $$=$1; $$->children.push_back($3); }

expression: expression PLUSOP expression {
                            $$ = new Node("AddExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression MINUSOP expression {
                            $$ = new Node("SubExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression MULTOP expression {
                            $$ = new Node("MultExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression AND expression {
                          $$ = new Node ("AndExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                        }
            | expression OR expression {
                          $$ = new Node ("OrExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                        }
            | expression LESSOP expression {
                            $$ = new Node("LessThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression GREATOP expression {
                            $$ = new Node("GreaterThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression EQUAL expression {
                          $$ = new Node ("EqualExpression", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                        }
            | expression LS expression RS {
                          $$ = new Node("ArrayAccess", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($3);
                        }
            | expression DOT LENGTH {
                          $$ = new Node("ArrayLength", "", yylineno);
                          $$->children.push_back($1);
                        }
            | expression DOT Identifier LP  RP {
                    $$ = new Node("MethodCall", "", yylineno);
                    $$->children.push_back($1);
                    $$->children.push_back($3);
                  }
            | THIS DOT Identifier LP expression_list RP {
                    $$ = new Node("ThisMethodCall", "", yylineno);
                    $$->children.push_back($3);
                    $$->children.push_back($5);
                  }
            | THIS DOT Identifier LP RP {
                    $$ = new Node("ThisMethodCall", "", yylineno);
                    $$->children.push_back($3);
                  }     
            | expression DOT Identifier LP expression_list RP {
                    $$ = new Node("MethodCall", "", yylineno);
                    $$->children.push_back($1);
                    $$->children.push_back($3);
                    $$->children.push_back($5);
                  }
            | INTLITERAL { $$ = new Node("INTLITERAL", $1, yylineno); }
            | TRUE { $$ = new Node("BooleanLiteral", "true", yylineno); }
            | FALSE { $$ = new Node("BooleanLiteral", "false", yylineno); }
            | THIS { $$ = new Node("ThisExpression", "", yylineno); }
            | NEW INT LS expression RS { $$ = new Node("NewArray", "", yylineno); $$->children.push_back($4); }
            | NEW Identifier LP RP { $$ = new Node("NewObject", "", yylineno); }
            | NOT expression  {
                          $$ = new Node("NotExpression", "", yylineno);
                          $$->children.push_back($2);
                        }
            | ID { $$ = new Node("Identifier", $1, yylineno); }
            | LP expression RP { $$ = $2; }
            ;
Identifier: ID { $$ = new Node("Identifier", $1, yylineno); };

%%
