%{
    #include <stdio.h>
    #include "node.h"
    #include "lex.yy.c"
%}

/* declared tokens */
%token INT
%token AND SUB MUL DIV





%%
Calc : 
| Exp {printf("= %d",$1);}
;



