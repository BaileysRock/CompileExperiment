
compile:lexical.l
	flex lexical.l
	gcc lex.yy.c -o lex -lfl
	./lex test.c