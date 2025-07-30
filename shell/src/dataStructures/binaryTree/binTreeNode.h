/* Generic binary tree node module */

/* DON'T NEED--Just collapse this into the parse tree */
#ifndef BIN_TREE_NODE_H
#define BIN_TREE_NODE_H

/* Base node definition */
typedef struct TreeLinks {
    BiTNode* parent;   /* Parent node */
    BiTNode* left;  /* Left child */
    BiTNode* right; /* Right child */
} TreeLinks;

typedef struct BiTNode {
    /* Data would go here, but we might have binary trees with different 
     * purposes, and I want the possible data/types listed in the file for that 
     * specific type */
    TreeLinks links;
} BiTNode;

struct BiTNode* biTNode_create();
void biTNode_destroy(BiTNode* node);

#endif