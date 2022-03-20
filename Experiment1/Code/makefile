
compile:lexical.l
	bison -d syntax.y
	flex lexical.l
	gcc lex.yy.c main.c -o lex -lfl
	./lex test.c