# include "semanticAnalysis.h"
# include <assert.h>
// 哈希函数的实现
unsigned int hash_pjw(char* name)
{
    unsigned int val = 0;
    unsigned int i;
    for(;*name;++name)
    {
        val = (val << 2) + *name;
        if (i==val & ~0x3fff)
            val = (val^(i>>12)) & 0x3fff;
    }
    return val;
}

// hashtable
Type symbolList[hashMapLength];

// 遍历树结构
void Traversal(Node* pNode)
{
    if (pNode == NULL)
        return;
    if (!strcmp(pNode->name,"ExtDef"))
        ExtDef(pNode);
    Traversal(pNode->childNode);
    Traversal(pNode->brotherNode);
}

Type newType(Kind kind, ...) {
    Type p = (Type)malloc(sizeof(struct Type_));
    assert(p != NULL);
    p->kind = kind;
    va_list vaList;
    switch (kind) {
        case BASIC:
            va_start(vaList, 1);
            p->u.basic = va_arg(vaList, basicType);
            break;
        case ARRAY:
            va_start(vaList, 2);
            p->u.array.elem = va_arg(vaList, Type);
            p->u.array.size = va_arg(vaList, int);
            break;
        case STRUCTURE:
            va_start(vaList, 2);
            p->u.structure.structName = va_arg(vaList, char*);
            p->u.structure.field = va_arg(vaList, FieldList);
            break;
        case FUNCTION:
            va_start(vaList, 3);
            p->u.function.argc = va_arg(vaList, int);
            p->u.function.argv = va_arg(vaList, FieldList);
            p->u.function.returnType = va_arg(vaList, pType);
            break;
    }
    va_end(vaList);
    return p;
}


// ExtDef -> Specifier ExtDecList SEMI
//         | Specifier SEMI
//         | Specifier FunDec CompSt
void ExtDef(Node* pNode)
{
    assert(pNode != NULL);
    Type SpecifierType = Specifier(pNode->childNode);
    char* secondName = pNode->childNode->brotherNode->name;
    if (!strcmp(secondName,"ExtDecList")){
        ExtDecList(pNode->childNode->next,SpecifierType);
    }
    else if (!strcmp(secondName, "FunDec")){
        FunDec(node->child->next, specifierType);
        CompSt(node->child->next->next, specifierType);
    }
    if (specifierType) deleteType(specifierType);

}


Type Specifier(Node* pNode) {
    assert(pNode != NULL);
    // Specifier -> TYPE
    //            | StructSpecifier

    pNode t = node->child;
    // Specifier -> TYPE
    if (!strcmp(t->name, "TYPE")) {
        if (!strcmp(t->val, "float")) {
            return newType(BASIC, FLOAT_TYPE);
        } else {
            return newType(BASIC, INT_TYPE);
        }
    }
    // Specifier -> StructSpecifier
    else {
        return StructSpecifier(t);
    }
}

