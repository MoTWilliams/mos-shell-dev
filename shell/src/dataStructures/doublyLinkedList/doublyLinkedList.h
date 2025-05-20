/* A doubly-linked list that can hold data of different types */
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "dListNode.h"

typedef struct DList {
        DLNode* head;
        DLNode* tail;
} DList;

DList* dList_create(void);
void dList_destroy(DList* list);

void dList_append(DList* list, NodeType type);

#endif