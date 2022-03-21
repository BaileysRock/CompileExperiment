
all:lexical.l
	bison -d syntax.y
	flex lexical.l
	gcc -g main.c node.c syntax.tab.c -o scanner -lfl