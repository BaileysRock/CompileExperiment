#include "node.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义符号表
struct symbol_table* table;

// 定义标识符类型
enum kind {
    // 基础
    BASIC, 
    // 数组
    ARRAY,
    // 结构体
    STRUCTURE, 
    // 函数
    FUNCTION   
};

// 定义基础数据类型
enum basic_type {
    INT_TYPE,
    FLOAT_TYPE 
};

// 定义各类语义分析错误类型
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

// 类型
struct type {
    // 标识符类型
    enum kind kind;
    union {
        // 基本
        enum basic_type basic;
        // 数组
        struct {
            struct type* element_type; // 数组类型
            int          size;         // 数组大小
        } array;
        // 结构体
        struct {
            char*         value; // 结构体的名字
            struct field* field; // 结构体各个字段指针
        } structure;
        // 函数
        struct {
            int           argc;        // 参数数量
            struct field* argv;        // 参数指针
            struct type*  return_type; // 返回类型
        } function;
    } u;
};

// 域：带有名字和类型的结构体
struct field {
    char*         value; // 名字
    struct type*  type;  // 类型
    struct field* next;  // 下一个域
};

// 符号表项目
struct table_item {
    struct field*      field; // 符号域信息
    struct table_item* next;  // 相同哈希值的下一个项目
};

// 符号表条目总数
#define SYMBOL_TABLE_SIZE 0x3fff

// 符号表，本实验中为哈希表
struct symbol_table {
    struct table_item** hash_array;
};

// 拷贝字符串src
char* copyStr(char* src);

// 打印错误信息
void printError(enum error_type type, int line, char* msg);

// 打印类型
void printKind(struct type* type);

// 递归进行语义分析
void Traversal(Node* node);

// 根据标识符种类kind和其余参数新建一个新类型
struct type* newType(enum kind kind, ...);

// 拷贝类型src
struct type* copyType(struct type* src);

// 检查两个类型type1和type2是否相同
// 若为BASIC: 检查是否同为 int 或 float
// 若为ARRAY: 检查元素的类型
// 若为STRUCT: 检查结构体名称
// 若为FUNCTION: 规定函数一定不同
// 相同则返回 1；否则返回 0
int isTypeSame(struct type* type1, struct type* type2);

// 使用name和type新建一个域
struct field* newField(char* name, struct type* type);

/// 拷贝域src
struct field* copyField(struct field* src);

/// 设置域field的名称字段为name
void setFieldName(struct field* field, char* name);

/// 根据域field创建一个新符号表条目
struct table_item* newTableItem(struct field* field);

// 判断符号表条目 item 是否是结构体
// 若是，则返回 1
// 否则返回 0
int isStruct(struct table_item* item);

// 初始化符号表
struct symbol_table* initTable();

// 计算字符串str的哈希值
unsigned getHashCode(char* str);

// 从符号表table中查找名称为name的符号条目指针。
// 若找不到，则返回 NULL
struct table_item* getTableItem(struct symbol_table* table, char* name);

// 检查item是否已经在符号表table中定义过。
// 如果已经定义，返回 1
// 否则返回 0
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
