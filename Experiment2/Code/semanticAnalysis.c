# include "semanticAnalysis.h"
# include <assert.h>
extern int yylineno;
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
FieldList* symbolList[hashMapLength];

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



// Specifier -> TYPE
//            | StructSpecifier
Type* Specifier(Node* pNode)
{
    assert(pNode != NULL);
    char* firstName = pNode->childNode->name;
    if (!strcmp(firstName,"TYPE")){
        if(!strcmp(pNode->childNode->value,"int"))
            return newTypeSpecifier(BASIC,INT_TYPE);
        else if(!strcmp(pNode->childNode->value,"float"))
            return newTypeSpecifier(BASIC,FLOAT_TYPE);
    }
}


// ExtDef -> Specifier ExtDecList SEMI
//         | Specifier SEMI
//         | Specifier FunDec CompSt
void ExtDef(Node* pNode)
{
    assert(pNode != NULL);
    Type* SpecifierType = Specifier(pNode->childNode);
    char* secondName = pNode->childNode->brotherNode->name;
    if (!strcmp(secondName, "ExtDecList")){
        // 传入的为ExtDecList节点
        ExtDecList(pNode->childNode->brotherNode,SpecifierType);
    }
    else if (!strcmp(secondName, "FunDec")){
        FunDec(pNode->childNode->brotherNode, SpecifierType);
        CompSt(pNode->childNode->brotherNode->brotherNode, SpecifierType);
    }
    // if (specifierType) deleteType(specifierType);

}





Type* newTypeSpecifier(typeKind typekind,basicType basictype)
{
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = typekind;
    type->u.basic = basictype;
    return type;
}


// ExtDecList -> VarDec     
//             | VarDec COMMA ExtDecList 
void ExtDecList(Node* pNode, Type* type)
{
    if(pNode != NULL)
    {
      if(pNode->childNode->brotherNode == NULL)
      {
          VarDec(pNode->childNode,type,NULL);
      }
      else
      {
          VarDec(pNode->childNode,type,NULL);
          ExtDecList(pNode->childNode->brotherNode->brotherNode,type);
      }
    }
}



void VarDec(Node* pNode, Type* type, Type* current)
{
    if(pNode != NULL)
    {
        if(!strcmp(pNode->childNode->name,"ID") && current == NULL)
        {
            FieldList* id = tokenType(pNode->childNode->value,type);
            insertHashMap(id);
        }
        else if (!strcmp(pNode->childNode->name,"ID") && current == NULL)
        {
            FieldList* id = arrayType(pNode->childNode->value,current);
            insertHashMap(id);            
        }
        else
        {
            char* number = pNode->childNode->brotherNode->brotherNode->value;
            for(int i = 0;i<strlen(number);i++)
            {
                if(number[i] == '.')
                    throwError(error12);
            }
            current = newArrayType(current,type);
            VarDec(pNode,type,current);
        }
    }
}





Type* newTypeVarDec(typeKind typekind,basicType basictype)
{
    Type* type = (Type*)malloc(sizeof(Type));
    type->kind = typekind;
    type->u.basic = basictype;
    return type;
}





// 构造数组的Type
Type* newArrayType(Type*currentType,Type* beforeType)
{
    Type* type = (Type*) malloc(sizeof(Type));
    type->kind = ARRAY;
    type->u.array.basictype = beforeType->u.basic;
    type->u.array.elem = currentType;
    
    if(currentType == NULL)
    {
        type->u.array.dimension = 1;
    }
    else
    {
        type->u.array.dimension = currentType->u.array.dimension + 1;
    }
    return type;
}

// 存入hashmap的type
FieldList* arrayType(char* name,Type* currentType)
{
    FieldList* fieldList = (FieldList*)malloc(sizeof(FieldList));
    fieldList->type = currentType;
    fieldList->name = name;
    fieldList->tail = NULL;
    return fieldList;
}


FieldList* tokenType(char* name,Type* type)
{
    FieldList* fieldList = (FieldList*)malloc(sizeof(FieldList));
    Type* typeTmp = (Type*)malloc(sizeof(Type));
    typeTmp->kind = type->kind;
    typeTmp->u.basic = type->u.basic;
    int nameLength = strlen(name)+1;
    char newName = (char*)malloc(sizeof(char)*nameLength);
    strncpy(newName,name,nameLength);
    fieldList->name = newName;
    fieldList->type = typeTmp;
    fieldList->tail = NULL;
    return fieldList;
}

void insertHashMap(FieldList* fieldList)
{
    unsigned int index = hash_pjw(fieldList->name);
    FieldList* temp = symbolList[index];
    if(temp == NULL)
    {
        symbolList[index] = fieldList;
    }
    else
    {
        while(temp->tail != NULL)
        {
            temp = temp -> tail;
        }
        temp -> tail = fieldList;
    }
}



void FunDec(Node* pNode, Type* type)
{

}


void CompSt(Node* pNode, Type* type)
{

}




void throwError(errorType errortype)
{
    char* errorInfos;
    int errorlabel;
    switch(errortype)
    {
        case error12:
            errorlabel = 12;
            errorInfos = "float number in []";
    }
    printf("Error type [%d] at Line %d:%s",errorlabel,yylineno,errorInfos);
}