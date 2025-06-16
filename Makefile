# Compiler targets
compiler: parser.tab.o lex.yy.c main.cc SymbolTable.o SemanticAnalyzer.o IRGenerator.o BytecodeGenerator.o
		g++ -g -w -ocompiler parser.tab.o lex.yy.c main.cc SymbolTable.o SemanticAnalyzer.o IRGenerator.o BytecodeGenerator.o -std=c++14

parser.tab.o: parser.tab.cc
		g++ -g -w -c parser.tab.cc -std=c++14

SymbolTable.o: SymbolTable.cc SymbolTable.h
		g++ -g -w -c SymbolTable.cc -std=c++14

SemanticAnalyzer.o: SemanticAnalyzer.cc SemanticAnalyzer.h
		g++ -g -w -c SemanticAnalyzer.cc -std=c++14

IRGenerator.o: IRGenerator.cc IRGenerator.h IR.h
		g++ -g -w -c IRGenerator.cc -std=c++14

BytecodeGenerator.o: BytecodeGenerator.cc BytecodeGenerator.h IR.h
		g++ -g -w -c BytecodeGenerator.cc -std=c++14

# Interpreter target
interpreter: BytecodeInterpreter.o interpreter_main.o
		g++ -g -w -ointerpreter BytecodeInterpreter.o interpreter_main.o -std=c++14

BytecodeInterpreter.o: BytecodeInterpreter.cc BytecodeInterpreter.h
		g++ -g -w -c BytecodeInterpreter.cc -std=c++14

interpreter_main.o: interpreter_main.cc BytecodeInterpreter.h
		g++ -g -w -c interpreter_main.cc -std=c++14

# Parser generation
parser.tab.cc: parser.yy
		bison parser.yy

lex.yy.c: lexer.flex parser.tab.cc
		flex lexer.flex

# Visualization targets
tree: 
		 dot -Tpdf tree.dot -otree.pdf

symboltable: 
		 dot -Tpdf symboltable.dot -osymboltable.pdf

cfg:
		 dot -Tpdf cfg.dot -ocfg.pdf

# Target to build everything
all: compiler interpreter

# Clean target
clean:
		rm -f parser.tab.* lex.yy.c* compiler interpreter *.o stack.hh position.hh location.hh 
		rm -f tree.dot tree.pdf symboltable.dot symboltable.pdf cfg.dot cfg.pdf
		rm -Rf compiler.dSYM interpreter.dSYM