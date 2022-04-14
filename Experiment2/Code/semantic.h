#include "node.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// 定义符号表
struct symbol_table* table;

/// 定义标识符类型
enum kind {
    BASIC, 
    // 数组类型
    ARRAY,
    // 结构体类型
    STRUCTURE, 
    // 函数类型
    FUNCTION   
};

// 定义基础数据类型
enum basic_type {
    INT_TYPE,
    FLOAT_TYPE 
};

/// 语义分析错误类型
enum error_type {
    UNDEFINE_VAR = 1,     // 变量未定义
    UNDEFINE_FUNC,        // 函数未定义
    REDEFINE_VAR,         // 变量重复定义
    REDEFINE_FUNC,        // 函数重复定义
    TYPE_MISMATCH_ASSIGN, // 类型不匹配的赋值
    LEFT_VAR_ASSIGN,      // 非法左值
    TYPE_MISMATCH_OP,     // 操作数类型不匹配
    TYPE_MISMATCH_RETURN, // 返回类型不匹配
    FUNC_AGRC_MISMATCH,   // 函数参数数量不正确
    NOT_A_ARRAY,          // 变量不是一个数组
    NOT_A_FUNC,           // 变量不是一个函数
    NOT_A_INT,            // 变量不是一个 int
    ILLEGAL_USE_DOT,      // 对非结构体变量使用 "." 运算符
    NONEXISTFIELD,        // 结构体没有对应的域
    REDEFINE_FEILD,       // 结构体中的域重复定义
    DUPLICATED_NAME,      // 结构体命名重复
    UNDEFINE_STRUCT       // 结构体未定义
};

/// 类型
struct type {
    // 标识符类型
    enum kind kind;
    union {
        // 基本类型
        enum basic_type basic;
        // 数组类型
        struct {
            struct type* element_type; // 元素类型
            int          size;         // 数组大小
        } array;

        // 结构体类型
        struct {
            char*         value;  // 结构体名
            struct field* field; // 结构体各个字段链表
        } structure;

        // 函数类型
        struct {
            int           argc;        // 参数数量
            struct field* argv;        // 参数链表
            struct type*  return_type; // 返回类型
        } function;
    } u;
};

/// 域：带有自己的名字和类型的“类型”
struct field {
    char*         value; // 名字
    struct type*  type; // 类型
    struct field* next; // 下一个域
};

/// 符号表项目
struct table_item {
    struct field*      field; // 符号域（类型）信息
    struct table_item* next;  // 相同哈希值的下一个项目
};

/// 符号表条目总数
#define SYM_TABLE_SIZE 0x3fff

/// 符号表，本质上是一个哈希表，用于根据符号名称快速找到对应的符号表条目
struct symbol_table {
    /// 一个符号表条目 item 在表中的索引位置的计算方法是：
    /// hash_array[HASH(`name`)] -> `name` 对应的符号表条目
    struct table_item** hash_array;
};

/// 拷贝字符串 \p src
char* copyStr(char* src);

/// 打印错误信息
void printError(enum error_type type, int line, char* msg);

/// 打印类型
void printKind(struct type* type);

/// 递归进行语义分析
void Traversal(Node* node);

/// 根据标识符种类kind和其余参数新建一个新类型
struct type* newType(enum kind kind, ...);

/// 拷贝类型src
struct type* copyType(struct type* src);

/// 检查两个类型type1和type2是否相同
/// BASIC: 检查是否同为 int 或 float
/// ARRAY: 检查元素的类型
/// STRUCTURE: 检查结构体名称
/// FUNCTION: 规定函数一定不同
/// 相同则返回 1；否则返回 0
int isTypeSame(struct type* type1, struct type* type2);

/// 根据名称name和类型type新建一个域
struct field* newField(char* name, struct type* type);

/// 拷贝域src
struct field* copyField(struct field* src);

/// 设置域field的名称字段为name
void setFieldName(struct field* field, char* name);

/// 根据域field创建一个新符号表条目
struct table_item* newTableItem(struct field* field);

/// 判断符号表条目 item 是否是结构体
/// 如果是，则返回 1；否则返回 0
int isStruct(struct table_item* item);

/// 初始化符号表
struct symbol_table* initTable();

/// 计算字符串str的哈希值
unsigned getHashCode(char* str);

/// 从符号表table中查找名称为name的符号条目指针。
///
/// 若找不到，则返回 NULL
struct table_item* getTableItem(struct symbol_table* table, char* name);

/// 检查item是否已经在符号表table中定义过。
///
/// 如果已经定义，返回 1；否则返回 0
int isTableItemRedefined(struct symbol_table* table, struct table_item* item);

/// 在符号表table 中插入新条目item
void addTableItem(struct symbol_table* table, struct table_item* item);

// 递归处理语法树
void ExtDef(Node* node);
void ExtDecList(Node* node, struct type* specifier);
struct type* Specifier(Node* node);
struct type* StructSpecifier(Node* node);
struct table_item* VarDec(Node* node, struct type* specifier);
void FunDec(Node* node, struct type* return_type);
void VarList(Node* node, struct table_item* func_item);
struct field* ParamDec(Node* node);
void CompSt(Node* node, struct type* return_type);
void StmtList(Node* node, struct type* return_type);
void Stmt(Node* node, struct type* return_type);
void DefList(Node* node, struct table_item* struct_item);
void Def(Node* node, struct table_item* struct_item);
void DecList(Node* node, struct type* specifier,struct table_item* struct_item);
void Dec(Node* node, struct type* specifier,struct table_item* struct_item);
struct type* Exp(Node* node);
void Args(Node* node, struct table_item* func_item);
