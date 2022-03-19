%option yywrap
%option yylineno
/* 一串不含空格的数字序列 */ 
/* 十进制 */
Decimal [-+]?(0|[1-9][0-9]*)
/* 八进制 */
Octal [-+]?0(0|[0-7]*)
/* 十六进制 */
Hexadecimal [-+]?(0x|0X)[0-9a-fA-F]+
INT [{Decimal}{Octal}{Hexadecimal}]
/* 由数字和一个小数点组成的数,小数点至少被一个数字围绕 */
FLOAT ([-+]?([0-9]*\.?[0-9]+|[0-9]+\.)|[-+]?[0-9]*\.[0-9]+[eE][+-]?[0-9]+)
/* 一个由52个大写字母或小写字母、10个数字和一个下划线字符组成的字符串,不能以字符串开头 */
ID [_a-zA-Z]+[_a-zA-Z0-9]{0,31}
/* 其他符号 */
SEMI ;
COMMA ,
ASSIGNOP =
RELOP > | < | >= | <= | == | !=
PLUS +
MINUS -
STAR *
DIV /
AND &&
OR ||
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
%%
{RC} {printf("%s\n",yytext); }
{INT} {printf("%s",yytext); }
{FLOAT} {printf("%s",yytext);}
{ID} {printf("%s",yytext);}

%%
int main(int argc,char** argv){
    printf("%s\n",argv[0]);
    printf("%s\n",argv[1]);
    yyin = fopen(argv[1],"r");
    yylex();
    return 0;
}


