#ifndef PARSE_TREE_NODE_H
#define PARSE_TREE_NODE_H

#include "binTreeNode.h"
#include "buffer.h"
#include "token.h"

/* Node data type labels */
typedef enum {
    /* Parse tree */
    TNODE_NONE, /* 0 */
    TNODE_CMD,  /* 1 */
    TNODE_SEQ,  /* 2: ; */
    TNODE_PIPE, /* 3: | */
    TNODE_OR,   /* 4: || */
    TNODE_BG,   /* 5: & */
    TNODE_AND   /* 6: && */
} PTNodeType;

/* Node data object--variable type */
typedef union {
    struct Buffer cmd;  /* An array of tokens */
    struct Token op;
} PTNodeData;

/* Parse tree node */
typedef struct PTNode {
    PTNodeType type;
    PTNodeData data;
    BiTNode node;
} PTNode;

PTNode* ptNode_create(PTNodeType type);
void ptNode_destroy(PTNode* node);

#endif