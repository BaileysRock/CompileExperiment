# ifndef SEMANTICANALYSIS_H
# define SEMANTICANALYSIS_H

# include "node.h"

// 定义hashmap的大小
# define hashMapLength 16384

typedef enum ErrorType{
    // 
    error1,
    error2,
    error3,
    error4,
    error5,
    error6,
    error7,
    error8,
    error9,
    error10,
    error11,
    // 数组访问操作符中出现非整数
    error12,
    error13,
    error14,
    error15,
    error16,
    error17
}errorType;



typedef enum TypeKind{
    BASIC,
    ARRAY,
    STRUCTURE,
    FUNCTION
}typeKind;

typedef enum BasicType{
    INT_TYPE,
    FLOAT_TYPE
}basicType;


// Type_数据类型
typedef struct Type_
{
    typeKind kind;
    union
    {
        // 基本类型

        basicType basic;
        // 数组的数据类型
        struct {
            basicType basictype;
            struct Type_* elem;
            int dimension;
        } array;

        struct {
            char* structName;
            struct FieldList_* field;
        } structure;

        struct {
            int argc;         // argument counter
            struct FieldList_* argv;   // argument vector
            struct Type_* returnType;  // returnType
        } function;
    } u;
}Type;

// FieldList_数据类型
typedef struct FieldList_
{
    // 域的名字
    char* name;
    // 域的类型
    struct Type_* type;
    // 下一个域
    struct FieldList_* tail;
}FieldList;

// 哈希函数
unsigned int hash_pjw(char* name);

// 遍历整棵树
void Traversal(Node* pNode);

// 报错信息
void Error();

// 提取specifier
Type* Specifier(Node* pNode);

// 提取extdef
void ExtDef(Node* pNode);

// 为specifier创建type
Type* newTypeSpecifier(typeKind typekind,basicType basictype);

// 提取ExtDecList
void ExtDecList(Node* pNode, Type* type);

Type* newTypeVarDec(typeKind typekind,basicType basictype);
// 
void VarDec(Node* pNode, Type* type, Type* current);

// 
void FunDec(Node* pNode, Type* type);

Type* newArrayType(Type*currentType,Type* beforeType);

FieldList* arrayType(char* name,Type* currentType);

FieldList* tokenType(char* name,Type* type);
// 
void CompSt(Node* pNode, Type* type);

void insertHashMap(FieldList* fieldList);



void throwError(errorType errortype);


# endif