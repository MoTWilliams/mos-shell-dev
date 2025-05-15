#include "doubleLinkedList.h"
#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>

/* Node Methods*/

static DListNode* dListNode_create(NodeType type);
static void dListNode_destroy(DListNode* node);

/* List Methods */

DList* dList_create() {
        // Allocate memory for the list
        DList* list = malloc(sizeof (DList));

        // Exit immediately if allocation fails
        if (!list) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the empty list
        list->head = NULL;
        list->tail = NULL;

        // Return the empty list
        return list;
}

void dList_destroy(DList* list) {
        // Nothing to do if the list is already free and empty
        if (!list) {
                return;
        }

        // Store a reference to the current node
        DListNode* current = list->head;

        // Destroy all nodes of the list first
        while (current) {
                // Store a reference to the next node
                DListNode* next = current->next;

                // Destroy the current node
                dListNode_destroy(current);

                // Move on to the next node
                current = next;
        }

        // Then destroy the list
        free(list);
}

void dList_append(DList* list, NodeType type) {
        // Create the new node of appropriate type
        DListNode* newNode = dListNode_create(type);

        // Special case: the list is empty--new node is now head
        if (!list->head) {
                list->head = newNode;
                list->tail = newNode;

        // Otherwise, add the new node to the end of the list
        } else {
                list->tail->next = newNode;
                newNode->prev = list->tail;
                list->tail = newNode;
        }
}

/* Node Methos Implementation */

static DListNode* dListNode_create(NodeType type) {
        // Allocate space for the node
        DListNode* node = malloc(sizeof(DListNode));

        if (!node) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Set the node's type
        node->type = type;
        
        // Allocate space for and initialize the contents
        switch (type) {
                case NODE_TOKEN:
                        node->data.token = token_create();
                        break;
        }

        // Initialize the node's pointers
        node->prev = NULL;
        node->next = NULL;

        // Return the initialized node
        return node;
}

static void dListNode_destroy(DListNode* node) {
        // Nothing to do if memory is already free and empty
        if (!node) {
                return;
        }

        // Destroy the contents with the appropriate methods
        switch (node->type) {
                case NODE_TOKEN:
                        token_destroy(node->data.token);
                        break;
        }

        // Free the node
        free(node);
}