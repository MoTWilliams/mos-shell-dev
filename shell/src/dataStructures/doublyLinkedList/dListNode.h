/* Doubly-linked list node module */
#ifndef D_LIST_NODE_H
#define D_LIST_NODE_H

#include "moErr.h"

/* Forward declarations */

/* Base node */
struct DLNode;
typedef struct DLNode DLNode;

/* Node types */
struct Token;

/* Node type labels */

typedef enum {
        NODE_TOKEN
        /* OTHER LABELS */
} NodeType;

/* Node data object--variable type */

typedef union {
        struct Token* token;
        /* Other node types */
} NodeData;

/* Base node definition */

struct DLNode {
        NodeType type;
        NodeData data;
        DLNode* next;
        DLNode* prev;
};

struct DLNode* dNode_create(NodeType type, Fatality isFatal);
void dNode_destroy(DLNode* node);

#endif