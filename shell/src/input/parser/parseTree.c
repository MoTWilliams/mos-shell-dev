#include "parseTree.h"
#include "pTreeNode.h"

#include "tokenList.h"

#include "mem.h"
#include "handleErrors.h"

PTree* pTree_create(struct TokenList* tList) {
        PTree* pTree = safeMalloc(sizeof(*pTree), FATAL);

        /* Token list for root node is just a chunk until divided and 
         * differentiated */
        pTree->ptRoot = ptNode_create(tList->head, PTNODE_CHUNK);
        pTree->ptRoot->last = tList->tail;

        return pTree;
}

void pTree_destroy(PTree* pTree) {
        /* Free all nodes */
        ptNode_destroy(pTree->ptRoot);

        /* Then free the tree */
        safeFree(pTree);
}