%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "node.h"
    #include "lex.yy.c"
    extern int syntaxError;
    Node* root;
    Node* p;
    Node* CreateNode(int lineNoTemp, NodeType nodeTypeTemp, char*nameTemp, char*tokenTemp);
    void InsertNode(Node* parent,Node* child);
%}
%union{
    struct treeNode* node; 
}


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

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program:   ExtDefList { p = CreateNode(@$.first_line, NON_TOKEN, "Program", NULL);  InsertNode(p, $1); $$ = p; root = p; }
    ;
ExtDefList: ExtDef ExtDefList { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDefList", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; } 
    | {$$ = NULL;}
    ;
ExtDef: Specifier ExtDecList SEMI { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDef", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Specifier SEMI { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDef", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | Specifier FunDec CompSt { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDef", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;
ExtDecList: VarDec { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDecList", NULL);  InsertNode(p, $1); $$ = p; }
    | VarDec COMMA ExtDecList { p = CreateNode(@$.first_line, NON_TOKEN, "ExtDecList", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;


Specifier: TYPE  { p = CreateNode(@$.first_line, NON_TOKEN, "Specifier", NULL);  InsertNode(p, $1); $$ = p; }
    | StructSpecifier { p = CreateNode(@$.first_line, NON_TOKEN, "Specifier", NULL);  InsertNode(p, $1); $$ = p; }
    ;
StructSpecifier: STRUCT OptTag LC DefList RC  { p = CreateNode(@$.first_line, NON_TOKEN, "StructSpecifier", NULL);  InsertNode(p, $1); InsertNode(p, $2);InsertNode(p, $3);InsertNode(p, $4);InsertNode(p, $5); $$ = p; }
    | STRUCT Tag { p = CreateNode(@$.first_line, NON_TOKEN, "StructSpecifier", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    ;
OptTag: ID { p = CreateNode(@$.first_line, NON_TOKEN, "OptTag", NULL);  InsertNode(p, $1); $$ = p; }
    | {$$ = NULL;}
    ;
Tag: ID { p = CreateNode(@$.first_line, NON_TOKEN, "Tag", NULL);  InsertNode(p, $1); $$ = p; }
    ; 


VarDec: ID { p = CreateNode(@$.first_line, NON_TOKEN, "VarDec", NULL);  InsertNode(p, $1); $$ = p; }
    | VarDec LB INT RB { p = CreateNode(@$.first_line, NON_TOKEN, "VarDec", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); $$ = p; }
    ;
FunDec: ID LP VarList RP { p = CreateNode(@$.first_line, NON_TOKEN, "FunDec", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); $$ = p; }
    | ID LP RP { p = CreateNode(@$.first_line, NON_TOKEN, "FunDec", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;
VarList: ParamDec COMMA VarList { p = CreateNode(@$.first_line, NON_TOKEN, "VarList", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | ParamDec { p = CreateNode(@$.first_line, NON_TOKEN, "VarList", NULL);  InsertNode(p, $1); $$ = p; }
    ;
ParamDec: Specifier VarDec { p = CreateNode(@$.first_line, NON_TOKEN, "ParamDec", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    ;


CompSt: LC DefList StmtList RC { p = CreateNode(@$.first_line, NON_TOKEN, "CompSt", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); $$ = p; }
    ;
StmtList: Stmt StmtList { p = CreateNode(@$.first_line, NON_TOKEN, "StmtList", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | {$$ = NULL;}
    ;
Stmt: Exp SEMI { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | CompSt { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); $$ = p; }
    | RETURN Exp SEMI { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | IF LP Exp RP Stmt { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); InsertNode(p, $5); $$ = p; }
    | IF LP Exp RP Stmt ELSE Stmt { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); InsertNode(p, $5); InsertNode(p, $6); InsertNode(p, $7); $$ = p; }
    | WHILE LP Exp RP Stmt { p = CreateNode(@$.first_line, NON_TOKEN, "Stmt", NULL);  InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); InsertNode(p, $5); $$ = p; }
    ;


DefList:  Def DefList  { p = CreateNode(@$.first_line, NON_TOKEN, "DefList", NULL); InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | {$$ = NULL;}
    ;
Def: Specifier DecList SEMI { p = CreateNode(@$.first_line, NON_TOKEN, "Def", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;
DecList: Dec { p = CreateNode(@$.first_line, NON_TOKEN, "DecList", NULL); InsertNode(p, $1); $$ = p; }
    | Dec COMMA DecList { p = CreateNode(@$.first_line, NON_TOKEN, "DecList", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;
Dec: VarDec { p = CreateNode(@$.first_line, NON_TOKEN, "Dec", NULL); InsertNode(p, $1); $$ = p; }
    | VarDec ASSIGNOP Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Dec", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    ;


Exp: Exp ASSIGNOP Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp AND Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp OR Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp RELOP Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp PLUS Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp MINUS Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp STAR Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp DIV Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | LP Exp RP { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | MINUS Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | NOT Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); $$ = p; }
    | ID LP Args RP { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); $$ = p; }
    | ID LP RP { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp LB Exp RB { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); InsertNode(p, $4); $$ = p; }
    | Exp DOT ID { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | ID { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); $$ = p; }
    | INT { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); $$ = p; }
    | FLOAT { p = CreateNode(@$.first_line, NON_TOKEN, "Exp", NULL); $$ = p; }
    ;
Args: Exp COMMA Args { p = CreateNode(@$.first_line, NON_TOKEN, "Args", NULL); InsertNode(p, $1); InsertNode(p, $2); InsertNode(p, $3); $$ = p; }
    | Exp { p = CreateNode(@$.first_line, NON_TOKEN, "Args", NULL); InsertNode(p, $1); $$ = p; }
    ;


%%



yyerror(char* msg){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, msg);
}