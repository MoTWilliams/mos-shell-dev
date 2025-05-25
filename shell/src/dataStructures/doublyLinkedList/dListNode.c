/* Doubly-linked list node methods */
#include "dListNode.h"
#include "token.h"
#include "mem.h"

DLNode* dNode_create(NodeType type) {
        /* Allocate space for the node */
        DLNode* node = moMalloc(sizeof(*node));

        /* Set the node's type */
        node->type = type;
        
        /* Allocate space for and initialize the contents */
        switch (type) {
                case NODE_TOKEN:
                        node->data.token = token_create();
                        break;
        }

        /* Initialize the node's pointers */
        node->prev = NULL;
        node->next = NULL;

        /* Return the initialized node */
        return node;
}

void dNode_destroy(DLNode* node) {
        /* Destroy the contents with the appropriate methods */
        switch (node->type) {
                case NODE_TOKEN:
                        token_destroy(node->data.token);
                        break;
                default:
                        /* Might add an error message here */
                        break;
        }

        /* Free the node */
        moFree(node);
}