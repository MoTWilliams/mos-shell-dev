#include "doublyLinkedList.h"
#include "mem.h"

DList* dList_create(void) {
        /* Allocate memory for the list */
        DList* list = moMalloc(sizeof(*list));

        /* Initialize the empty list's pointers */
        list->head = NULL;
        list->tail = NULL;

        /* Return the empty list */
        return list;
}

void dList_destroy(DList* list) {
        DLNode* current = NULL; /* Will hold a reference to the current node */

        /* If NULL is passed into dList_destroy, attempting to assign current  
         * to the head of the list will segfault, so we'll just exit early */
        if (!list) {
                return;
        }

        /* Grab the current node */
        current = list->head;

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
        moFree(list);
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