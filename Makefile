compiler: lex.yy.c parser.tab.o main.cc SymbolTable.o SemanticAnalyzer.o
		g++ -g -w -ocompiler parser.tab.o lex.yy.c main.cc SymbolTable.o SemanticAnalyzer.o -std=c++14
parser.tab.o: parser.tab.cc
		g++ -g -w -c parser.tab.cc -std=c++14
SymbolTable.o: SymbolTable.cc SymbolTable.h
		g++ -g -w -c SymbolTable.cc -std=c++14
SemanticAnalyzer.o: SemanticAnalyzer.cc SemanticAnalyzer.h
		g++ -g -w -c SemanticAnalyzer.cc -std=c++14
parser.tab.cc: parser.yy
		bison parser.yy
lex.yy.c: lexer.flex parser.tab.cc
		flex lexer.flex
tree: 
		 dot -Tpdf tree.dot -otree.pdf
symboltable: 
		 dot -Tpdf symboltable.dot -osymboltable.pdf
clean:
		rm -f parser.tab.* lex.yy.c* compiler stack.hh position.hh location.hh tree.dot tree.pdf
		rm -f tree.dot tree.pdf symboltable.dot symboltable.pdf *.o
		rm -R compiler.dSYM