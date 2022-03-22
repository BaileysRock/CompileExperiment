all:lexical syntax parser

CC = gcc
FLEX = flex
BISON = bison


# 所有.c文件
CompileFiles = $(wildcard *.c)
# 生成的可重定位文件
ObjFiles = $(patsubst %.c,%.o,$(CompileFiles) )
# 获取词法文件
LexicalFile = $(wildcard *.l)
# 获取语法文件
SyntaxFile = $(wildcard *.y)

# 获取
LexicalFileC = lex.yy.c
SyntaxFileC = syntax.tab.c

lexical:$(LexicalFile)
	$(FLEX) -o $(LexicalFileC) $(LexicalFile)
syntax:$(SyntaxFile)
	$(BISON) -d $(SyntaxFile)
parser:$(SyntaxFileC) node.c main.c
	$(CC) $(SyntaxFileC) node.c main.c -o parser -lfl

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h 

