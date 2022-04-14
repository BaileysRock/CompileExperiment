#include "semantic.h"
#include <stdlib.h>
#include <string.h>



char* copyStr(char* src) {
    if (src == NULL) {
        return NULL;
    }

    int   len = strlen(src) + 1;
    char* cpy = (char*)malloc(sizeof(char) * len);
    strncpy(cpy, src, len);

    return cpy;
}

void printError(enum error_type type, int line, char* msg) {
    printf("Error type %d at Line %d: %s\n", type, line, msg);
}

void print_func_error(Node* node, struct table_item* func_item) {
    printf("Error type %d at Line %d: ", FUNC_AGRC_MISMATCH, node->lineNo);
    printf("Function \"%s(", func_item->field->value);
    struct field* field_temp = func_item->field;
    while (field_temp) {
        printKind(field_temp->type->u.function.argv->type);
        field_temp = field_temp->next;
    }
    printf(")\" is not applicable for arguments \"(");
    Node* args_node_temp = node;
    while (args_node_temp->childNode->brotherNode) {
        struct type* args_type_temp = Exp(args_node_temp->childNode);
        printKind(args_type_temp);
        printf(", ");
        args_node_temp =
            args_node_temp->childNode->brotherNode->brotherNode;
    }
    struct type* args_type_temp = Exp(args_node_temp->childNode);
    printKind(args_type_temp);
    printf(")\".\n");
}

void printKind(struct type* type) {
    switch (type->kind) {
    case BASIC:
        if (type->u.basic == INT_TYPE) {
            printf("int");
        } else {
            printf("float");
        }
        break;
    case ARRAY:
        printKind(type->u.array.element_type);
        break;
    case STRUCTURE:
        printf("struct %s", type->u.structure.value);
        break;
    case FUNCTION:
        printf("function");
        break;
    };
}

void Traversal(Node* node) {
    if (node == NULL)
        return;

    if (!strcmp(node->value, "ExtDef"))
        ExtDef(node);

    Traversal(node->childNode);
    Traversal(node->brotherNode);
}

struct type* newType(enum kind kind, ...) {
    struct type* type = (struct type*)malloc(sizeof(struct type));

    type->kind = kind;

    va_list vl;

    switch (kind) {
    case BASIC:
        va_start(vl, 1);
        type->u.basic = va_arg(vl, enum basic_type);
        va_end(vl);
        break;
    case ARRAY:
        va_start(vl, 2);
        type->u.array.element_type = va_arg(vl, struct type*);
        type->u.array.size         = va_arg(vl, int);
        va_end(vl);
        break;
    case STRUCTURE:
        va_start(vl, 2);
        type->u.structure.value  = va_arg(vl, char*);
        type->u.structure.field = va_arg(vl, struct field*);
        va_end(vl);
        break;
    case FUNCTION:
        va_start(vl, 3);
        type->u.function.argc        = va_arg(vl, int);
        type->u.function.argv        = va_arg(vl, struct field*);
        type->u.function.return_type = va_arg(vl, struct type*);
        va_end(vl);
        break;
    }

    return type;
}

struct type* copyType(struct type* src) {
    if (src == NULL) {
        return NULL;
    }

    struct type* type = (struct type*)malloc(sizeof(struct type));

    type->kind = src->kind;

    switch (type->kind) {
    case BASIC:
        type->u.basic = src->u.basic;
        break;
    case ARRAY:
        type->u.array.element_type = copyType(src->u.array.element_type);
        type->u.array.size         = src->u.array.size;
        break;
    case STRUCTURE:
        type->u.structure.value  = copyStr(src->u.structure.value);
        type->u.structure.field = copyField(src->u.structure.field);
        break;
    case FUNCTION:
        type->u.function.argc        = src->u.function.argc;
        type->u.function.argv        = copyField(src->u.function.argv);
        type->u.function.return_type = copyType(src->u.function.return_type);
        break;
    }

    return type;
}

int isTypeSame(struct type* type1, struct type* type2) {
    if (type1 == NULL || type2 == NULL)
        return 1;
    if (type1->kind == FUNCTION || type2->kind == FUNCTION)
        return 0;
    if (type1->kind != type2->kind)
        return 0;
    else {
        switch (type1->kind) {
        case BASIC:
            return type1->u.basic == type2->u.basic;
        case ARRAY:
            return isTypeSame(type1->u.array.element_type,
                                type2->u.array.element_type);
        case STRUCTURE:
            return !strcmp(type1->u.structure.value, type2->u.structure.value);
        case FUNCTION:
            // 这个分支已经被前边的 if 覆盖到了，所以一定不会走到这里
            return 0;
        }
    }
}

struct field* newField(char* name, struct type* type) {
    struct field* field = (struct field*)malloc(sizeof(struct field));

    field->value = copyStr(name);
    field->type = type;
    field->next = NULL;

    return field;
}

struct field* copyField(struct field* src) {
    struct field* head = NULL;
    struct field* curr = NULL;
    struct field* copy = src;

    while (copy) {
        if (!head) {
            head = newField(copy->value, copyType(copy->type));
            curr = head;
            copy = copy->next;
        } else {
            curr->next = newField(copy->value, copyType(copy->type));
            curr       = curr->next;
            copy       = copy->next;
        }
    }

    return head;
}

void setFieldName(struct field* field, char* name) {
    if (field->value != NULL) {
        free(field->value);
    }

    field->value = copyStr(name);
}

struct table_item* newTableItem(struct field* field) {
    struct table_item* item = (struct table_item*)malloc(sizeof(struct table_item));

    item->field = field;
    item->next  = NULL;

    return item;
}

int isStruct(struct table_item* item) {
    if (item == NULL)
        return 0;
    if (item->field->type->kind != STRUCTURE)
        return 0;
    if (item->field->type->u.structure.value)
        return 0;
    return 1;
}

struct symbol_table* initTable() {
    struct symbol_table* table =
        (struct symbol_table*)malloc(sizeof(struct symbol_table));

    table->hash_array = (struct table_item**)malloc(sizeof(struct table_item*) *
                                                    SYMBOL_TABLE_SIZE);
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        table->hash_array[i] = NULL;
    }

    return table;
};

unsigned getHashCode(char* str) {
    unsigned hash = 0;

    for (; *str; ++str) {
        hash = (hash << 2) + *str;

        unsigned i;
        if ((i = hash & ~SYMBOL_TABLE_SIZE))
            hash = (hash ^ (i >> 12)) & SYMBOL_TABLE_SIZE;
    }

    return hash;
}

struct table_item* getTableItem(struct symbol_table* table, char* name) {
    unsigned           hash_code = getHashCode(name);
    struct table_item* item_head = table->hash_array[hash_code];

    while (item_head) {
        if (!strcmp(item_head->field->value, name)) {
            return item_head;
        }

        item_head = item_head->next;
    }

    return NULL;
}

int isTableItemRedefined(struct symbol_table* table, struct table_item* item) {
    struct table_item* temp = getTableItem(table, item->field->value);

    while (temp) {
        if (!strcmp(temp->field->value, item->field->value)) {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

void addTableItem(struct symbol_table* table, struct table_item* item) {
    unsigned hash_code = getHashCode(item->field->value);

    item->next                   = table->hash_array[hash_code];
    table->hash_array[hash_code] = item;
}

void ExtDef(Node* node) {
    // ExtDef : Specifier ExtDecList SEMI
    //        | Specifier SEMI
    //        | Specifier FunDec CompSt

    struct type* specifierType     = Specifier(node->childNode);
    char*        second_child_name = node->childNode->brotherNode->value;

    if (!strcmp(second_child_name, "ExtDecList")) {
        // ExtDef : Specifier ExtDecList SEMI

        ExtDecList(node->childNode->brotherNode, specifierType);
    } else if (!strcmp(second_child_name, "FunDec")) {
        // ExtDef : Specifier FunDec CompSt

        FunDec(node->childNode->brotherNode, specifierType);
        CompSt(node->childNode->brotherNode->brotherNode, specifierType);
    }
}

void ExtDecList(Node* node, struct type* specifier) {
    // ExtDecList : VarDec
    //            | VarDec COMMA ExtDecList

    Node* temp = node;
    while (temp) {
        struct table_item* item = VarDec(temp->childNode, specifier);

        if (isTableItemRedefined(table, item)) {
            char msg[100] = {0};
            sprintf(msg, "Redefined variable \"%s\".", item->field->value);
            printError(REDEFINE_VAR, temp->lineNo, msg);
        } else {
            addTableItem(table, item);
        }

        if (temp->childNode->brotherNode) {
            temp = temp->brotherNode->brotherNode->childNode;
        } else {
            break;
        }
    }
}

struct type* Specifier(Node* node) {
    // Specifier : TYPE
    //           | StructSpecifier

    Node* lc = node->childNode;

    if (lc->nodeType == TOKEN_TYPE) {
        // Specifier : TYPE

        if (!strcmp(lc->value, "float")) {
            return newType(BASIC, FLOAT_TYPE);
        } else {
            return newType(BASIC, INT_TYPE);
        }
    } else {
        // Specifier : StructSpecifier

        return StructSpecifier(lc);
    }
}

struct type* StructSpecifier(Node* node) {
    // StructSpecifier : STRUCT OptTag LC DefList RC
    //                 | STRUCT Tag

    Node* second_child = node->childNode->brotherNode;
    struct type* return_type  = NULL;

    if (strcmp(second_child->value, "Tag")) {
        // StructSpecifier : STRUCT OptTag LC DefList RC

        struct table_item* struct_item =
            newTableItem(newField("", newType(STRUCTURE, NULL, NULL)));

        // OptTag -> ID | empty
        // Tag -> ID

        if (!strcmp(second_child->value, "OptTag")) {
            setFieldName(struct_item->field, second_child->childNode->value);
            second_child = second_child->brotherNode;
        }

        if (!strcmp(second_child->brotherNode->value, "DefList")) {
            DefList(second_child->brotherNode, struct_item);
        }

        if (isTableItemRedefined(table, struct_item)) {
            char msg[100] = {0};
            sprintf(msg, "Duplicated name \"%s\".", struct_item->field->value);
            printError(DUPLICATED_NAME, node->lineNo, msg);
        } else {
            return_type = newType(
                STRUCTURE, copyStr(struct_item->field->value),
                copyField(struct_item->field->type->u.structure.field));

            if (!strcmp(node->childNode->brotherNode->value, "OptTag")) {
                addTableItem(table, struct_item);
            }
        }
    } else {
        // StructSpecifier : STRUCT Tag

        struct table_item* struct_item =
            getTableItem(table, second_child->childNode->value);

        if (struct_item == NULL || !isStruct(struct_item)) {
            char msg[100] = {0};
            sprintf(msg, "Undefined structure \"%s\".",
                    second_child->childNode->value);
            printError(UNDEFINE_STRUCT, node->lineNo, msg);
        } else {
            return_type = newType(
                STRUCTURE, copyStr(struct_item->field->value),
                copyField(struct_item->field->type->u.structure.field));
        }
    }

    return return_type;
}

struct table_item* VarDec(Node* node, struct type* specifier) {
    // VarDec : ID
    //        | VarDec LB INT RB

    // 获取到 ID
    Node* id = node;
    while (id->childNode) {
        id = id->childNode;
    }

    struct table_item* item = newTableItem(newField(id->value, NULL));

    if (node->childNode->nodeType == TOKEN_ID) {
        // VarDec : ID

        item->field->type = copyType(specifier);
    } else {
        // VarDec : VarDec LB INT RB

        Node* vardec = node->childNode;
        struct type* temp   = specifier;

        while (vardec->brotherNode) {
            item->field->type =
                newType(ARRAY, copyType(temp),
                         atoi(vardec->brotherNode->brotherNode->value));
            temp   = item->field->type;
            vardec = vardec->childNode;
        }
    }

    return item;
}

void FunDec(Node* node, struct type* return_type) {
    // FunDec : ID LP VarList RP
    //        | ID LP RP

    struct table_item* item = newTableItem(
        newField(node->childNode->value,
                  newType(FUNCTION, 0, NULL, copyType(return_type))));

    if (!strcmp(node->childNode->brotherNode->brotherNode->value,
                "VarList")) {
        // FunDec : ID LP VarList RP

        VarList(node->childNode->brotherNode->brotherNode, item);
    }

    // FunDec : ID LP RP 不需要处理

    // 检查重定义
    if (isTableItemRedefined(table, item)) {
        char msg[100] = {0};
        sprintf(msg, "Redefined function \"%s\".", item->field->value);
        printError(REDEFINE_FUNC, node->lineNo, msg);
        item = NULL;
    } else {
        addTableItem(table, item);
    }
}

void VarList(Node* node, struct table_item* func_item) {
    // VarList : ParamDec COMMA VarList
    //         | ParamDec

    int           argc = 0;
    Node*  lc   = node->childNode;
    struct field* curr = NULL;

    // VarList : ParamDec
    struct field* paramdec                  = ParamDec(lc);
    func_item->field->type->u.function.argv = copyField(paramdec);
    curr = func_item->field->type->u.function.argv;
    argc++;

    // VarList : ParamDec COMMA VarList
    while (lc->brotherNode) {
        lc       = lc->brotherNode->brotherNode->childNode;
        paramdec = ParamDec(lc);

        if (paramdec) {
            curr->next = copyField(paramdec);
            curr       = curr->next;
            argc++;
        }
    }

    func_item->field->type->u.function.argc = argc;
}

struct field* ParamDec(Node* node) {
    // ParamDec : Specifier VarDec

    struct type*       specifier_type = Specifier(node->childNode);
    struct table_item* item =
        VarDec(node->childNode->brotherNode, specifier_type);

    if (isTableItemRedefined(table, item)) {
        char msg[100] = {0};
        sprintf(msg, "Redefined variable \"%s\".", item->field->value);
        printError(REDEFINE_VAR, node->lineNo, msg);
        return NULL;
    } else {
        addTableItem(table, item);
        return item->field;
    }
}

void CompSt(Node* node, struct type* return_type) {
    // CompSt : LC DefList StmtList RC

    Node* temp = node->childNode->brotherNode;

    if (!strcmp(temp->value, "DefList")) {
        DefList(temp, NULL);
        temp = temp->brotherNode;
    }

    if (!strcmp(temp->value, "StmtList")) {
        StmtList(temp, return_type);
    }
}

void StmtList(Node* node, struct type* return_type) {
    // StmtList : Stmt StmtList
    //          | empty

    while (node) {
        Stmt(node->childNode, return_type);
        node = node->childNode->brotherNode;
    }
}

void Stmt(Node* node, struct type* return_type) {
    // Stmt : Exp SEMI
    //      | CompSt
    //      | RETURN Exp SEMI
    //      | IF LP Exp RP Stmt
    //      | IF LP Exp RP Stmt ELSE Stmt
    //      | WHILE LP Exp RP Stmt

    struct type* exp_type = NULL;

    if (!strcmp(node->childNode->value, "Exp")) {
        // Stmt : Exp SEMI

        exp_type = Exp(node->childNode);
    } else if (!strcmp(node->childNode->value, "CompSt")) {
        // Stmt : CompSt

        CompSt(node->childNode, return_type);
    } else if (!strcmp(node->childNode->value, "RETURN")) {
        // Stmt : RETURN Exp SEMI

        exp_type = Exp(node->childNode->brotherNode);

        if (!isTypeSame(return_type, exp_type)) {
            printError(TYPE_MISMATCH_RETURN, node->lineNo,
                        "Type mismatched for return.");
        }
    } else if (!strcmp(node->childNode->value, "IF")) {
        // Stmt : IF LP Exp RP Stmt

        Node* stmt = node->childNode->brotherNode->brotherNode
                                ->brotherNode->brotherNode;
        exp_type = Exp(node->childNode->brotherNode->brotherNode);
        Stmt(stmt, return_type);

        if (stmt->brotherNode != NULL) {
            // Stmt : IF LP Exp RP Stmt ELSE Stmt

            Stmt(stmt->brotherNode->brotherNode, return_type);
        }
    } else if (!strcmp(node->childNode->value, "WHILE")) {
        // Stmt : WHILE LP Exp RP Stmt

        exp_type = Exp(node->childNode->brotherNode->brotherNode);
        Stmt(node->childNode->brotherNode->brotherNode->brotherNode
                 ->brotherNode,
             return_type);
    }
}

void DefList(Node* node, struct table_item* struct_item) {
    // DefList : Def DefList
    //         | empty

    while (node) {
        Def(node->childNode, struct_item);
        node = node->childNode->brotherNode;
    }
}

void Def(Node* node, struct table_item* struct_item) {
    // Def : Specifier DecList SEMI

    struct type* dectype = Specifier(node->childNode);
    DecList(node->childNode->brotherNode, dectype, struct_item);
}

void DecList(Node* node, struct type* specifier,
             struct table_item* struct_item) {
    // DecList : Dec
    //         | Dec COMMA DecList

    Node* temp = node;
    while (temp) {
        Dec(temp->childNode, specifier, struct_item);
        if (temp->childNode->brotherNode) {
            temp = temp->childNode->brotherNode->brotherNode;
        } else {
            break;
        }
    }
}

void Dec(Node* node, struct type* specifier,
         struct table_item* struct_info) {
    // Dec : VarDec
    //     | VarDec ASSIGNOP Exp

    if (!node->childNode->brotherNode) {
        // Dec : VarDec

        if (struct_info != NULL) {
            struct table_item* dec_item = VarDec(node->childNode, specifier);
            struct field*      payload  = dec_item->field;
            struct field*      struct_field =
                struct_info->field->type->u.structure.field;
            struct field* last = NULL;

            while (struct_field) {
                if (!strcmp(payload->value, struct_field->value)) {
                    //出现重定义，报错
                    char msg[100] = {0};
                    sprintf(msg, "Redefined field \"%s\".",
                            dec_item->field->value);
                    printError(REDEFINE_FEILD, node->lineNo, msg);
                    return;
                } else {
                    last         = struct_field;
                    struct_field = struct_field->next;
                }
            }

            if (last == NULL) {
                struct_info->field->type->u.structure.field =
                    copyField(dec_item->field);
            } else {
                last->next = copyField(dec_item->field);
            }
        } else {
            struct table_item* decitem = VarDec(node->childNode, specifier);
            if (isTableItemRedefined(table, decitem)) {
                // 出现冲突，报错
                char msg[100] = {0};
                sprintf(msg, "Redefined variable \"%s\".",
                        decitem->field->value);
                printError(REDEFINE_VAR, node->lineNo, msg);
            } else {
                addTableItem(table, decitem);
            }
        }
    } else {
        // Dec : VarDec ASSIGNOP Exp

        if (struct_info != NULL) {
            // 结构体内不能赋值，报错
            printError(REDEFINE_FEILD, node->lineNo,
                        "Illegal initialize variable in struct.");
        } else {
            // 判断赋值类型是否相符
            // 如果成功，注册该符号
            struct table_item* dec_item = VarDec(node->childNode, specifier);
            struct type*       exp_type =
                Exp(node->childNode->brotherNode->brotherNode);
            if (isTableItemRedefined(table, dec_item)) {
                // 出现冲突，报错
                char msg[100] = {0};
                sprintf(msg, "Redefined variable \"%s\".",
                        dec_item->field->value);
                printError(REDEFINE_VAR, node->lineNo, msg);
            }
            if (!isTypeSame(dec_item->field->type, exp_type)) {
                // 类型不相符，报错
                printError(TYPE_MISMATCH_ASSIGN, node->lineNo,
                            "struct type mis matched for assignment.");
            }
            if (dec_item->field->type && dec_item->field->type->kind == ARRAY) {
                // 报错，对非 BASIC 类型赋值
                printError(TYPE_MISMATCH_ASSIGN, node->lineNo,
                            "Illegal initialize variable.");
            } else {
                addTableItem(table, dec_item);
            }
        }
    }
}

struct type* Exp(Node* node) {
    // Exp : Exp ASSIGNOP Exp
    //     | Exp AND Exp
    //     | Exp OR Exp
    //     | Exp RELOP Exp
    //     | Exp PLUS Exp
    //     | Exp MINUS Exp
    //     | Exp STAR Exp
    //     | Exp DIV Exp
    //     | Exp LB Exp RB
    //     | Exp DOT ID
    //     | LP Exp RP
    //     | MINUS Exp
    //     | NOT Exp
    //     | ID LP Args RP
    //     | ID LP RP
    //     | ID
    //     | INT
    //     | FLOAT

    Node* lc = node->childNode;

    if (!strcmp(lc->value, "Exp")) {
        // Exp : Exp ASSIGNOP Exp
        //     | Exp AND Exp
        //     | Exp OR Exp
        //     | Exp RELOP Exp
        //     | Exp PLUS Exp
        //     | Exp MINUS Exp
        //     | Exp STAR Exp
        //     | Exp DIV Exp
        //     | Exp LB Exp RB
        //     | Exp DOT ID

        if (strcmp(lc->brotherNode->value, "LB") &&
            strcmp(lc->brotherNode->value, "DOT")) {
            // Exp : Exp ASSIGNOP Exp
            //     | Exp AND Exp
            //     | Exp OR Exp
            //     | Exp RELOP Exp
            //     | Exp PLUS Exp
            //     | Exp MINUS Exp
            //     | Exp STAR Exp
            //     | Exp DIV Exp

            struct type* expl        = Exp(lc);
            struct type* expr        = Exp(lc->brotherNode->brotherNode);
            struct type* return_type = NULL;

            if (!strcmp(lc->brotherNode->value, "ASSIGNOP")) {
                // Exp : Exp ASSIGNOP Exp

                // 检查左值
                Node* lc_lc = lc->childNode;
                if (lc_lc->nodeType == TOKEN_FLOAT || lc_lc->nodeType == TOKEN_INT) {
                    // 报错，左值
                    printError(LEFT_VAR_ASSIGN, lc->lineNo,
                                "The left-hand side of an assignment must be "
                                "a variable.");
                } else if (lc_lc->nodeType == TOKEN_ID ||
                           !strcmp(lc_lc->brotherNode->value, "LB") ||
                           !strcmp(lc_lc->brotherNode->value, "DOT")) {
                    if (!isTypeSame(expl, expr)) {
                        // 报错，类型不匹配
                        printError(TYPE_MISMATCH_ASSIGN, lc->lineNo,
                                    "Type mismatched for assignment.");
                    } else
                        return_type = copyType(expl);
                } else {
                    //报错，左值
                    printError(LEFT_VAR_ASSIGN, lc->lineNo,
                                "The left-hand side of an assignment must be "
                                "a variable.");
                }
            } else {
                // Exp : Exp AND Exp
                //     | Exp OR Exp
                //     | Exp RELOP Exp
                //     | Exp PLUS Exp
                //     | Exp MINUS Exp
                //     | Exp STAR Exp
                //     | Exp DIV Exp

                if (expl && expr &&
                    (expl->kind == ARRAY || expr->kind == ARRAY)) {
                    // 报错，数组，结构体运算
                    printError(TYPE_MISMATCH_OP, lc->lineNo,
                                "Type mismatched for operands.");
                } else if (!isTypeSame(expl, expr)) {
                    // 报错，类型不匹配
                    printError(TYPE_MISMATCH_OP, lc->lineNo,
                                "Type mismatched for operands.");
                } else {
                    if (expl && expr) {
                        return_type = copyType(expl);
                    }
                }
            }

            return return_type;
        } else {
            if (!strcmp(lc->brotherNode->value, "LB")) {
                // Exp -> Exp LB Exp RB

                struct type* arr_name  = Exp(lc);
                struct type* arr_index = Exp(lc->brotherNode->brotherNode);
                struct type* return_type = NULL;

                if (arr_name && arr_name->kind != ARRAY) {
                    // 报错，非数组使用 [] 运算符
                    char msg[100] = {0};
                    sprintf(msg, "\"%s\" is not an array.",
                            lc->childNode->value);
                    printError(NOT_A_ARRAY, lc->lineNo, msg);
                } else if (!arr_index || arr_index->kind != BASIC ||
                           arr_index->u.basic != INT_TYPE) {
                    // 报错，不用 int 索引 []
                    char msg[100] = {0};
                    sprintf(msg, "\"%s\" is not an integer.",
                            lc->brotherNode->brotherNode->childNode->value);
                    printError(NOT_A_INT, lc->lineNo, msg);
                } else {
                    return_type = copyType(arr_name->u.array.element_type);
                }

                return return_type;
            } else {
                // Exp -> Exp DOT ID

                struct type* struct_type = Exp(lc);
                struct type* return_type = NULL;

                if (!struct_type || struct_type->kind != STRUCTURE ||
                    !struct_type->u.structure.value) {
                    // 对非结构体使用 "." 运算符
                    // 调用printError进行报错处理
                    printError(ILLEGAL_USE_DOT, lc->lineNo,
                                "Illegal use of \".\".");
                } else {
                    Node* struct_member =
                        lc->brotherNode->brotherNode;
                    struct field* struct_field = struct_type->u.structure.field;

                    while (struct_field != NULL) {
                        if (!strcmp(struct_field->value, struct_member->value)) {
                            break;
                        }
                        struct_field = struct_field->next;
                    }
                    if (struct_field == NULL) {
                        // 没有可以匹配的域
                        char msg[100] = {0};
                        sprintf(msg, "Non-existent field \"%s\".",
                                struct_member->value);
                        printError(NONEXISTFIELD, lc->lineNo, msg);
                    } else {
                        return_type = copyType(struct_field->type);
                    }
                }

                return return_type;
            }
        }
    } else if (!strcmp(lc->value, "MINUS") || !strcmp(lc->value, "NOT")) {
        // Exp : MINUS Exp
        //     | NOT Exp

        struct type* simgle_exp  = Exp(lc->brotherNode);
        struct type* return_type = NULL;

        if (!simgle_exp || simgle_exp->kind != BASIC) {
            // 数组，结构体运算
            // 调用printError进行报错处理
            printf("Error type %d at Line %d: %s.\n", 7, lc->lineNo,
                   "TYPE_MISMATCH_OP");
        } else {
            return_type = copyType(simgle_exp);
        }

        return return_type;
    } else if (!strcmp(lc->value, "LP")) {
        // Exp : LP Exp RP
        return Exp(lc->brotherNode);
    } else if (lc->nodeType == TOKEN_ID && lc->brotherNode) {
        // Exp : ID LP Args RP
        //     | ID LP RP

        struct table_item* func_item = getTableItem(table, lc->value);

        // 没有这个函数
        if (func_item == NULL) {
            char msg[100] = {0};
            sprintf(msg, "Undefined function \"%s\".", lc->value);
            printError(UNDEFINE_FUNC, node->lineNo, msg);
            return NULL;
        } else if (func_item->field->type->kind != FUNCTION) {
            char msg[100] = {0};
            sprintf(msg, "\"i\" is not a function.", lc->value);
            printError(NOT_A_FUNC, node->lineNo, msg);
            return NULL;
        } else if (!strcmp(lc->brotherNode->brotherNode->value, "Args")) {
            // Exp : ID LP Args RP

            Args(lc->brotherNode->brotherNode, func_item);
            return copyType(func_item->field->type->u.function.return_type);
        } else {
            // Exp : ID LP RP

            if (func_item->field->type->u.function.argc != 0) {
                char msg[100] = {0};
                sprintf(msg,
                        "too few arguments to function \"%s\", except %d args.",
                        func_item->field->value,
                        func_item->field->type->u.function.argc);
                printError(FUNC_AGRC_MISMATCH, node->lineNo, msg);
            }
            return copyType(func_item->field->type->u.function.return_type);
        }
    } else if (lc->nodeType == TOKEN_ID) {
        // Exp : ID

        struct table_item* item = getTableItem(table, lc->value);
        if (item == NULL || isStruct(item)) {
            char msg[100] = {0};
            sprintf(msg, "Undefined variable \"%s\".", lc->value);
            printError(UNDEFINE_VAR, lc->lineNo, msg);
            return NULL;
        } else {
            return copyType(item->field->type);
        }
    } else {
        if (lc->nodeType == TOKEN_FLOAT) {
            // Exp : FLOAT

            return newType(BASIC, FLOAT_TYPE);
        } else {
            // Exp : INT

            return newType(BASIC, INT_TYPE);
        }
    }
}

void Args(Node* node, struct table_item* func_item) {
    // Args : Exp COMMA Args
    //      | Exp

    Node*  temp = node;
    struct field* argv = func_item->field->type->u.function.argv;

    while (temp) {
        if (argv == NULL) {
            print_func_error(node, func_item);
            break;
        }

        struct type* arg_type = Exp(temp->childNode);

        if (!isTypeSame(arg_type, argv->type)) {
            char msg[100] = {0};
            sprintf(msg, "Function \"%s\" is not applicable for arguments.",
                    func_item->field->value);
            printError(FUNC_AGRC_MISMATCH, node->lineNo, msg);
            return;
        }

        argv = argv->next;
        if (temp->childNode->brotherNode) {
            temp = temp->childNode->brotherNode->brotherNode;
        } else {
            break;
        }
    }

    if (argv != NULL) {
        print_func_error(node, func_item);
    }
}
