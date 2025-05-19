// Node module for doubly-linked list
#ifndef D_LIST_NODE_H
#define D_LIST_NODE_H

/* Forward declarations */

// Base node
struct DLNode;
typedef struct DLNode DLNode;

// Node types
struct Token;
typedef struct Token Token;

/* Node type labels */

typedef enum {
        NODE_TOKEN
        // OTHER LABELS
} NodeType;

/* Node data object--variable type */

typedef union {
        Token* token;
        // Other node types
} NodeData;

/* Base node definition */

struct DLNode {
        NodeType type;
        NodeData data;
        DLNode* next;
        DLNode* prev;
};

DLNode* dNode_create(NodeType type);
void dNode_destroy(DLNode* node);

#endif