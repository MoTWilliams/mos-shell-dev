#include "parseTree.h"
#include "pTreeNode.h"
#include "ptNodeTypes.h"

#include "tokenList.h"

#include "mem.h"

PTree* pTree_create(TokList* toks) {
        PTree* pTree = safeMalloc(sizeof(*pTree), FATAL);

        /* Create an base default chunk node--we will start dividing this into
         * the tree later. This line assigns the root node's first and last
         * token to the head of the token list. */
        pTree->root = ptNode_create(toks->tokList->head, PTNODE_CHUNK);

        /* This line assigns the root's last token to the tail of the token
         * list, so as to emcompass all of the extracted tokens. */
        pTree->root->data.chunk->last = toks->tokList->tail;

        /* Return the tree, ready to divide into branches and leaves */
        return pTree;
}

static void pTree_destroy_recursive(PTNode* node);

void pTree_destroy(PTree* pTree) {
        /* Destroy all nodes first */
        pTree_destroy_recursive(pTree->root);

        /* Then free the tree */
        safeFree(pTree);
}

static void pTree_destroy_recursive(PTNode* node) {
        if (!node) return;

        /* Free children first */
        pTree_destroy_recursive(node->lhs);
        pTree_destroy_recursive(node->rhs);

        /* Then free the node itself */
        ptNode_destroy(node);
}