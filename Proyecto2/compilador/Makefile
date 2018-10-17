CC=g++

all: yaccfile lexfile main

main: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o main -std=c++11

lexfile: compiler.l
	lex compiler.l

yaccfile: compiler.y
		yacc -d compiler.y

clean:
	rm lex.yy.c y.tab.h y.tab.c
