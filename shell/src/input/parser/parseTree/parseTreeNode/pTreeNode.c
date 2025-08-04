/* Parse tree node methods */

#include "pTreeNode.h"
#include "ptNodeTypes.h"
#include "tokenList.h"

#include "mem.h"
#include "handleErrors.h"

PTNode* ptNode_create(Token* curTok, PTNodeType type) {
        PTNode* node = safeMalloc(sizeof(*node), FATAL);

        node->type = type;
        node->lhs = NULL;
        node->rhs = NULL;

        switch (node->type) {
                case PTNODE_OP:
                        node->data.op = op_create(curTok);
                        break;
                case PTNODE_CMD:
                        node->data.cmd = command_create(curTok);
                        break;
                /* Default to chunk node */
                default:
                        node->data.chunk = chunk_create(curTok);
                        break;
        }

        return node;
}

void ptNode_destroy(PTNode* node) {
        /* Free the data first */
        switch (node->type) {
                case PTNODE_CHUNK:
                        chunk_destroy(node->data.chunk);
                        break;
                case PTNODE_OP:
                        op_destroy(node->data.op);
                        break;
                case PTNODE_CMD:
                        command_destroy(node->data.cmd);
                        break;
        }

        /* Zero out attributes */
        node->type = PTNODE_CHUNK;
        node->lhs = NULL;
        node->rhs = NULL;

        /* Then free the node */
        safeFree(node);
}

Token* ptNode_append(PTNode* node, Token* curTok) {
        /* Only attempt to append to chunk or command nodes */
        switch (node->type) {
                case PTNODE_CHUNK:
                        chunk_append(node->data.chunk, curTok);
                        break;
                case PTNODE_CMD:
                        command_append(node->data.cmd, curTok);
                        break;
                default:
                        REPORT_ERR(FATAL, ERR_NODE_TYPE, "Append failed");
                        break;
        }
}