%{
    #include <stdio.h>
    #include "node.h"
    #include "lex.yy.c"
%}

/* declared tokens */

%token <node> INT
%token <node> FLOAT
%token <node> ID
%token <node> SEMI
%token <node> COMMA
%token <node> ASSIGNOP
%token <node> RELOP
%token <node> PLUS
%token <node> MINUS
%token <node> STAR
%token <node> DIV
%token <node> AND
%token <node> OR
%token <node> DOT
%token <node> NOT
%token <node> TYPE
%token <node> LP
%token <node> RP
%token <node> LB
%token <node> RB
%token <node> LC
%token <node> RC
%token <node> STRUCT
%token <node> RETURN
%token <node> IF
%token <node> ELSE
%token <node> WHILE

%type <node> Program
%type <node> ExtDefList 
%type <node> ExtDef
%type <node> ExtDecList

%type <node> StructSpecifier
%type <node> Specifier 
%type <node> OptTag
%type <node> Tag

%type <node> VarDec
%type <node> FunDec
%type <node> VarList
%type <node> ParamDec

%type <node> Stmt
%type <node> StmtList
%type <node> CompSt

%type <node> DefList
%type <node> Def
%type <node> Dec
%type <node> DecList

%type <node> Args
%type <node> Exp



%%
Program : ExtDefList
    ;
ExtDefList : ExtDef ExtDefList 
    |
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;


Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    |
    ;
Tag : ID
    ;


VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;


CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    |
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;


DefList : Def DefList
    | 
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;


Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID 
    | INT
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;


%%


