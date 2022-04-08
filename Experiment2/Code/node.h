#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义词法、语法错误
#define BOOL int
#define TRUE 1
#define FALSE 0

// node的类型
typedef enum nodeType{
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_TYPE,
    TOKEN_OTHER,
    NON_TOKEN
}NodeType;

typedef struct treeNode{
    // 词法单位的行
    int lineNo;
    // 词法单位的类型
    NodeType nodeType;
    // 词法单位的名称
    char* name;
    // 词法单位若为终结符则保存他的内容
    char* value;
    // 非终结符的孩子节点
    struct treeNode* childNode;
    // 非终结符的下一个兄弟节点
    struct treeNode* brotherNode;
}Node;

// 创建节点
Node* CreateNode(int lineNoTemp, NodeType nodeTypeTemp, char*nameTemp, char*tokenTemp);

// 插入子节点
void InsertNode(Node* parent,Node* child);

// 删除语法树
void DeleteTree(Node* root);

// 打印树
void PrintTree(Node* currentNode, int height);

#endif