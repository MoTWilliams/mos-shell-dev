// Doubly-linked list node methods
#include "dListNode.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>

DLNode* dNode_create(NodeType type) {
        // Allocate space for the node
        DLNode* node = malloc(sizeof(DLNode));

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

void dNode_destroy(DLNode* node) {
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