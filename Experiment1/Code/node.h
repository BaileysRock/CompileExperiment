#ifndef NODE_H
#define NODE_H

#include <stdio.h>

// node的类型
typedef enum nodeType{
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_TYPE,
    TOKEN_OTHER,
    NON_TOKEN
}NodeType;

typedef struct node{
    // 词法单位的行
    int lineNo;
    // 词法单位的类型
    NodeType nodeType;
    // 词法单位的值
    char* value;
    // 非终结符的孩子节点
    struct node* childNode;
    // 非终结符的下一个兄弟节点
    struct node* brotherNode;
}Node;

// 创建节点
static inline Node* CreateNode(int lineNoTemp, NodeType nodeTypeTemp, char* valueTemp, Node* childNodeTemp, Node* brotherNodeTemp);

// 删除语法树
static inline void DeleteTree(Node* root);

// 打印树
static inline void PrintTree(Node* root, int height);

#endif