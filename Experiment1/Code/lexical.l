%option yylineno

%{
    #include "node.h"
    #include "syntax.tab.h"

    extern unsigned lexError;
    int lines = 0;
    int yycolumn = 1;

    #define YY_USER_ACTION \
        yylloc.first_line=yylloc.last_line=yylineno; \
        yylloc.first_column=yycolumn; \
        yylloc.last_column=yycolumn+yyleng-1; \
        yycolumn+=yyleng;
%}


digit [0-9]

/* 一串不含空格的数字序列 */ 
/* 十进制 */
Decimal [-+]?(0|[1-9][0-9]*)
/* 八进制 */
Octal [-+]?0(0|[0-7]*)
/* 十六进制 */
Hexadecimal [-+]?(0x|0X)[0-9a-fA-F]+
/* INT [{Decimal}{Octal}{Hexadecimal}] */
INT 0|[1-9]+[0-9]*
/* 由数字和一个小数点组成的数,小数点至少被一个数字围绕 */
FLOAT ([-+]?([0-9]*\.?[0-9]+|[0-9]+\.)|[-+]?[0-9]*\.[0-9]+[eE][+-]?[0-9]+)
/* 一个由52个大写字母或小写字母、10个数字和一个下划线字符组成的字符串,不能以字符串开头 */
ID [_a-zA-Z]+[_a-zA-Z0-9]{0,31}
/* 其他符号 */
SEMI ;
COMMA ,
ASSIGNOP =
DOT \.

PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
NOT !

LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
TYPE int|float
RELOP >|<|>=|<=|==|!=
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while

%%
\t+ {;}
{IF} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "IF", yytext); return IF; }
{ELSE} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "ELSE", yytext); return ELSE; }
{WHILE} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "WHILE", yytext); return WHILE; }
{STRUCT} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "STRUCT", yytext); return STRUCT; }
{RETURN} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "RETURN", yytext); return RETURN; }
{RELOP} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "RELOP", yytext); return RELOP; }

{PLUS} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "PLUS", yytext); return PLUS; }
{MINUS} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "MINUS", yytext); return MINUS; }
{STAR} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "STAR", yytext); return STAR; }
{DIV} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "DIV", yytext); return DIV; }
{AND} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "AND", yytext); return AND; }
{OR} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "OR", yytext); return OR; }
{NOT} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "NOT", yytext); return NOT; }

{SEMI} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "SEMI", yytext); return SEMI; }
{COMMA} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "COMMA", yytext); return COMMA; }
{ASSIGNOP} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "ASSIGNOP", yytext); return ASSIGNOP; }
{DOT} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "DOT", yytext); return DOT; }

{LP} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "LP", yytext); return LP; }
{RP} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "RP", yytext); return RP; }
{LB} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "LB", yytext); return LB; }
{RB} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "RB", yytext); return RB; }
{LC} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "LC", yytext); return LC; }
{RC} { yylval.node = CreateNode(yylineno, TOKEN_OTHER, "RC", yytext); return RC; }

{TYPE} { yylval.node = CreateNode(yylineno, TOKEN_TYPE, "TYPE", yytext); return TYPE; }
{ID} { yylval.node = CreateNode(yylineno, TOKEN_ID, "ID", yytext); return ID; }
{INT} { yylval.node = CreateNode(yylineno, TOKEN_INT, "INT", yytext); return INT; }
{FLOAT} { yylval.node = CreateNode(yylineno, TOKEN_FLOAT, "FLOAT", yytext); return FLOAT; }


%%


// int main(int argc,char** argv){
//     printf("%s\n",argv[0]);
//     printf("%s\n",argv[1]);
//     yyin = fopen(argv[1],"r");
//     yylex();
//     return 0;
// }


