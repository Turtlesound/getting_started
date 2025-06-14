%top{
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
    int lexical_errors = 0;
}
%option yylineno noyywrap nounput batch noinput stack 
%%

"+"                     {if(USE_LEX_ONLY) {printf("PLUSOP ");} else {return yy::parser::make_PLUSOP(yytext);}}
"-"                     {if(USE_LEX_ONLY) {printf("SUBOP ");} else {return yy::parser::make_SUBOP(yytext);}}
"*"                     {if(USE_LEX_ONLY) {printf("MULTOP ");} else {return yy::parser::make_MULTOP(yytext);}}
"/"                     {if(USE_LEX_ONLY) {printf("DIVOP ");} else {return yy::parser::make_DIVOP(yytext);}}
"("                     {if(USE_LEX_ONLY) {printf("LP ");} else {return yy::parser::make_LP(yytext);}}
")"                     {if(USE_LEX_ONLY) {printf("RP ");} else {return yy::parser::make_RP(yytext);}}
"<"                     {if(USE_LEX_ONLY) {printf("LESSOP ");} else {return yy::parser::make_LESSOP(yytext);}}
">"                     {if(USE_LEX_ONLY) {printf("GREATOP ");} else {return yy::parser::make_GREATOP(yytext);}}
"&&"                    {if(USE_LEX_ONLY) {printf("AND");} else {return yy::parser::make_AND(yytext);}}
"||"                    {if(USE_LEX_ONLY) {printf("OR");} else {return yy::parser::make_OR(yytext);}}
"="                     {if(USE_LEX_ONLY) {printf("ASSIGN");} else {return yy::parser::make_ASSIGN(yytext);}}
"=="                    {if(USE_LEX_ONLY) {printf("EQUAL");} else {return yy::parser::make_EQUAL(yytext);}}
"]"                     {if(USE_LEX_ONLY) {printf("RS ");} else {return yy::parser::make_RS(yytext);}}
"["                     {if(USE_LEX_ONLY) {printf("LS ");} else {return yy::parser::make_LS(yytext);}}
0|[1-9][0-9]*           {if(USE_LEX_ONLY) {printf("INTLITERAL ");} else {return yy::parser::make_INTLITERAL(yytext);}}
"true"                  {if(USE_LEX_ONLY) {printf("TRUE ");} else {return yy::parser::make_TRUE(yytext);}}
"false"                 {if(USE_LEX_ONLY) {printf("FALSE ");} else {return yy::parser::make_FALSE(yytext);}}
"."                     {if(USE_LEX_ONLY) {printf("DOT ");} else {return yy::parser::make_DOT(yytext);}}
","                     {if(USE_LEX_ONLY) {printf("COMMA ");} else {return yy::parser::make_COMMA(yytext);}}
"this"                  {if(USE_LEX_ONLY) {printf("THIS ");} else {return yy::parser::make_THIS(yytext);}}
"new"                   {if(USE_LEX_ONLY) {printf("NEW ");} else {return yy::parser::make_NEW(yytext);}}
"!"                     {if(USE_LEX_ONLY) {printf("NOT ");} else {return yy::parser::make_NOT(yytext);}}
"public"                {if(USE_LEX_ONLY) {printf("PUBLIC ");} else {return yy::parser::make_PUBLIC(yytext);}}
"class"                 {if(USE_LEX_ONLY) {printf("CLASS ");} else {return yy::parser::make_CLASS(yytext);}}
"static"                {if(USE_LEX_ONLY) {printf("STATIC ");} else {return yy::parser::make_STATIC(yytext);}}
"void"                  {if(USE_LEX_ONLY) {printf("VOID ");} else {return yy::parser::make_VOID(yytext);}}
"main"                  {if(USE_LEX_ONLY) {printf("MAIN ");} else {return yy::parser::make_MAIN(yytext);}}
"String"                {if(USE_LEX_ONLY) {printf("STRING ");} else {return yy::parser::make_STRING(yytext);}}
"string"                {if(USE_LEX_ONLY) {printf("STRING ");} else {return yy::parser::make_STRING(yytext);}}
"int"                   {if(USE_LEX_ONLY) {printf("INT ");} else {return yy::parser::make_INT(yytext);}}
"boolean"               {if(USE_LEX_ONLY) {printf("BOOLEAN ");} else {return yy::parser::make_BOOLEAN(yytext);}}
"if"                    {if(USE_LEX_ONLY) {printf("IF ");} else {return yy::parser::make_IF(yytext);}}
"else"                  {if(USE_LEX_ONLY) {printf("ELSE ");} else {return yy::parser::make_ELSE(yytext);}}
"while"                 {if(USE_LEX_ONLY) {printf("WHILE ");} else {return yy::parser::make_WHILE(yytext);}}
"System.out.println"    {if(USE_LEX_ONLY) {printf("SYSTEMOUTPRINTLN ");} else {return yy::parser::make_SYSTEMOUTPRINTLN(yytext);}}
"return"                {if(USE_LEX_ONLY) {printf("RETURN ");} else {return yy::parser::make_RETURN(yytext);}}
"length"                {if(USE_LEX_ONLY) {printf("LENGTH ");} else {return yy::parser::make_LENGTH(yytext);}}
"{"                     {if(USE_LEX_ONLY) {printf("LBRACE ");} else {return yy::parser::make_LBRACE(yytext);}}
"}"                     {if(USE_LEX_ONLY) {printf("RBRACE ");} else {return yy::parser::make_RBRACE(yytext);}}
";"                     {if(USE_LEX_ONLY) {printf("SEMICOLON ");} else {return yy::parser::make_SEMICOLON(yytext);}}
[a-zA-Z][a-zA-Z0-9_]*   {if(USE_LEX_ONLY) {printf("ID ");} else {return yy::parser::make_ID(yytext);}}
[ \t\n\r]+              {}
"//"[^\n]*              {}
.                       { if(!lexical_errors) fprintf(stderr, "Lexical errors found! See the logs below: \n"); fprintf(stderr,"\t@error at line %d. Character %s is not recognized\n", yylineno, yytext); lexical_errors = 1;}
<<EOF>>                  {return yy::parser::make_END();}
%%