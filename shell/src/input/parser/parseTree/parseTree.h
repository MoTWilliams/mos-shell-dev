#ifndef PARSE_TREE_H
#define PARSE_TREE_H

struct PTNode;
struct TokList;

typedef struct PTree {
        PTNode* root;
} PTree;

PTree* pTree_create(TokList* toks);
void pTree_destroy(PTree* tree);

#endif