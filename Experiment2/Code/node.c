#include "node.h"

// 创建节点
// 包括终结符和非终结符
// token为可选的
Node* CreateNode(int lineNoTemp, NodeType nodeTypeTemp, char*nameTemp, char*tokenTemp)
{
    Node* newNode = (Node*) malloc(sizeof(Node));
    int nameLength = strlen(nameTemp)+1;
    char* name = (char*) malloc(sizeof(char)*nameLength);
    char* value = NULL;
    int tokenLength = 0;
    if(tokenTemp)
    {
        tokenLength = strlen(tokenTemp) + 1;
        value = (char*)malloc(sizeof(char)*tokenLength);
    }
    if(!newNode || !name || (tokenTemp && !value))
    {
        perror("error!");
        exit(-1);
    }
    newNode->brotherNode = NULL;
    newNode->childNode = NULL;
    newNode->lineNo = lineNoTemp;
    newNode->nodeType = nodeTypeTemp;
    strncpy(name, nameTemp, nameLength);
    newNode->name = name;
    if(tokenTemp != NULL)
        strncpy(value, tokenTemp, tokenLength);
    newNode->value = value;
    return newNode;
}

// 插入子节点
void InsertNode(Node* parent,Node* child)
{
    if (parent->childNode == NULL)
    {
        parent->childNode = child;
    }
    else
    {
        Node* p = parent->childNode;
        // while(!p->brotherNode)
        while(p->brotherNode != NULL)
            p = p->brotherNode;
        p->brotherNode = child;        
    }
}

// 删除语法树
void DeleteTree(Node* root)
{
    if(root != NULL)
    {
        DeleteTree(root->childNode);
        DeleteTree(root->brotherNode);
        // 这个地方free之后不知道为什么会报重复free
        // 因此改为指向NULL
        // free(root->name);
        // free(root->value);
        // free(root);
        root = NULL;
    }
}

// 打印树
void PrintTree(Node* currentNode, int height)
{ 
    if (currentNode == NULL) {
        return; 
    }
    if (height == 0)
        printf("\n");
    for (int i = 0; i < height; i++) {
        printf("  ");
    }
    printf("%s", currentNode->name);
    if (currentNode->nodeType == NON_TOKEN) {
        printf(" (%d)", currentNode->lineNo);
    } else if (currentNode->nodeType == TOKEN_TYPE || currentNode->nodeType == TOKEN_ID || currentNode->nodeType == TOKEN_INT) {
        printf(": %s", currentNode->value);
    } else if (currentNode->nodeType == TOKEN_FLOAT) {
        printf(": %lf", atof(currentNode->value));
    }
    printf("\n");
    PrintTree(currentNode->childNode, height + 1);
    PrintTree(currentNode->brotherNode, height);
}


