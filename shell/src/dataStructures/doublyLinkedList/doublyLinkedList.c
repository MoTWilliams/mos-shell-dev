#include "doublyLinkedList.h"

#include <stdio.h>
#include <stdlib.h>

DList* dList_create(void) {
        /* Allocate memory for the list */
        DList* list = malloc(sizeof(*list));

        /* Exit immediately if allocation fails */
        if (!list) {
                perror("Memory allocation failed");
                exit(1);
        }

        /* Initialize the empty list's pointers */
        list->head = NULL;
        list->tail = NULL;

        /* Return the empty list */
        return list;
}

void dList_destroy(DList* list) {
        /* Store a reference to the current node */
        DLNode* current = list->head;

        /* Nothing to do if the list is already free and empty */
        if (!list) {
                return;
        }

        /* Destroy all nodes of the list first */
        while (current) {
                /* Store a reference to the next node */
                DLNode* next = current->next;

                /* Destroy the current node */
                dNode_destroy(current);

                /* Move on to the next node */
                current = next;
        }

        /* Then destroy the list */
        free(list);
}

void dList_append(DList* list, NodeType type) {
        /* Create the new node of appropriate type */
        DLNode* newNode = dNode_create(type);

        /* Special case: the list is empty--new node is now head */
        if (!list->head) {
                list->head = newNode;
                list->tail = newNode;

        /* Otherwise, add the new node to the end of the list */
        } else {
                list->tail->next = newNode;
                newNode->prev = list->tail;
                list->tail = newNode;
        }
}