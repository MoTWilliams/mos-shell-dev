// A doubly-linked list that can hold data of different types
#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

struct Token;
typedef struct Token Token;

typedef enum {
        NODE_TOKEN
} NodeType;

typedef union {
        Token* token;
} NodeData;

typedef struct DListNode {
        NodeType type;
        NodeData data;
        struct DListNode* next;
        struct DListNode* prev;
} DListNode;

typedef struct DList {
        DListNode* head;
        DListNode* tail;
} DList;

DList* dList_create();
void dList_destroy(DList* list);

void dList_append(DList* list, NodeType type);

#endif