# include "semanticAnalysis.h"

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
FieldList symbolList[hashMapLength];

void Traversal(Node* pNode)
{
    
    printf("%s",pNode->name);
    
}


