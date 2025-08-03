#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "tokenList.h"
#include "pTreeNode.h"

typedef struct PTree {
        PTNode* root;
} PTree;

PTree* pTree_create(TokList* toks);
void pTree_destroy(PTree* tree);

#endif