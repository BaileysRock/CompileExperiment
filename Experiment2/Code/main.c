#include "node.h"
#include "syntax.tab.h"
#include "semantic.h"
BOOL lexError = FALSE;
BOOL synError = FALSE;

extern Node* root;
extern int yylineno;
extern int yyparse();
extern void yyrestart(FILE*);


int main(int argc, char** argv) {
    // 如果没有文件则需要手动输入
    if (argc < 2) {
        yyparse();
        return 1;
    }
    // 如果有多个文件
    for(int i = 1; i < argc; i++)
    {
        FILE* f = fopen(argv[i], "r");
        if(!f)
        {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yyparse();
        fclose(f);
        if(!lexError && !synError)
        { 
            table = init_table();
            Traversal(root);
        }
        // PrintTree(root,0);
        DeleteTree(root);
        yylineno = 1;
        lexError = FALSE;
        synError = FALSE;
    }
    return 0;
}
