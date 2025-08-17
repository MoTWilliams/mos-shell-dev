#ifndef PARSE_TREE_H
#define PARSE_TREE_H

struct PTree;
typedef struct PTree PTree;

struct PTNode;
struct TokenList;

struct PTree {
        struct PTNode* ptRoot;
};

PTree* pTree_create(struct TokenList* tList);
void pTree_destroy(PTree* pTree);

#endif