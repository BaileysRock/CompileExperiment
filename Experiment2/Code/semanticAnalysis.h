# ifndef SEMANTICANALYSIS_H
# define SEMANTICANALYSIS_H

# include "node.h"

// 定义hashmap的大小
# define hashMapLength 16384

typedef struct Type_* Type;

typedef struct FieldList_* FieldList;

typedef enum TypeKind{
    BASIC,
    ARRAY,
    STRUCTURE,
    FUNCTION
}Kind;
typedef enum BasicType{
    INT_TYPE,
    FLOAT_TYPE
}basicType;


// Type_数据类型
struct Type_
{
    Kind kind;
    union
    {
        // 基本类型
        basicType basic;
        struct {
            Type elem;
            int size;
        } array;
        struct {
            char* structName;
            FieldList field;
        } structure;

        struct {
            int argc;          // argument counter
            FieldList argv;   // argument vector
            Type returnType;  // returnType
        } function;
    } u;
};

// FieldList_数据类型
struct FieldList_
{
    // 域的名字
    char* name;
    // 域的类型
    Type type;
    // 下一个域
    FieldList tail;
};

// 哈希函数
unsigned int hash_pjw(char* name);

// 遍历整棵树
void Traversal(Node* pNode);

// 报错信息
void Error();

void ExtDef(Node* pNode);


# endif