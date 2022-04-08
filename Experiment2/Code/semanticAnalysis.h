# include "node.h"

// 定义hashmap的大小
# define hashMapLength 16384

typedef struct Type_* Type;

typedef struct FieldList_* FieldList;

// Type_数据类型
struct Type_
{
    enum { BASIC, ARRAY, STRUCTURE} kind;
    union
    {
        // 基本类型
        int basic;
        struct {
            Type elem;
            int size;
        } array;
        FieldList structure;
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




