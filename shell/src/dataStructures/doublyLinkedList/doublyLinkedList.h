/* A doubly-linked list that can hold data of different types */
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "dListNode.h"

#include "handleErrors.h"

typedef struct DList {
        DLNode* head;
        DLNode* tail;
} DList;

DList* dList_create(Fatality isFatal);
void dList_destroy(DList* list);

DLNode* dList_append(DList* list, NodeType type, Fatality isFatal);

#endif