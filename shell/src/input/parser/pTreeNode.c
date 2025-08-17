#include "pTreeNode.h"
#include "parsingIterator.h"

#include "mem.h"
#include "handleErrors.h"

PTNode* ptNode_create(struct Token* curTok, PTNodeType type) {
        PTNode* ptNode = safeMalloc(sizeof(*ptNode), FATAL);

        /* Set matadata */
        ptNode->ptnType = type;
        ptNode->cType = CMD_NOTYPE;

        /* Add the first token to the node */
        ptNode->first = curTok;
        ptNode->last = curTok;

        /* Nullify the tree links for now */
        ptNode->lhs = NULL;
        ptNode->rhs = NULL;

        return ptNode;
}

void ptNode_destroy(PTNode* ptNode) {
        if (!ptNode) return;
        
        /* Set all fields back to default state */
        ptNode->ptnType = PTNODE_CHUNK;
        ptNode->cType = CMD_NOTYPE;
        ptNode->first = NULL;
        ptNode->last = NULL;

        /* Destroy children */
        ptNode_destroy(ptNode->lhs);
        ptNode_destroy(ptNode->rhs);

        /* Free the node itself */
        safeFree(ptNode);
}

static Keyword getClosingKeyType(Token* curTok);
static void build_RECURSIVE(Token curTok);

void ptNode_build(PTNode* ptNode, TokType delimiter) {
        Token* curTok = ptNode->first;
        int nestDepth = 0;
        Keyword closeKType = KEY_NOTYPE;

        /* Stop adding to the node at delimiter at level zero--semicolons mean 
         * something else inside of compound commands */

        /* TODO: I need a stack for the closing keywords. This will most likely involve recursion, if I don't want to involve a buffer or another linked list or something. For now, it is sleep time */
        while (curTok->tType != delimiter && nestDepth > 0) {


                /* If the current token is an opening keyword, increase depth */
                closeKType = getClosingKeyType(curTok);
                if (closeKType != KEY_NOTYPE) nestDepth++;

                /* If the current token closes the compound command, decrease 
                 * depth */
                if (curTok->kType == closeKType) nestDepth--;

                /* Move on to the next token */
                ptNode->last = curTok->next;
        }
}

static Keyword getClosingKeyType(Token* curTok) {
        switch (curTok->kType) {
                case KEY_IF: return KEY_FI;
                case KEY_FOR: case KEY_WHILE: case KEY_UNTIL: return KEY_DONE;
                case KEY_CASE: return KEY_ESAC;
                case KEY_L_PAREN: return KEY_R_PAREN;
                case KEY_L_BRACE: return KEY_R_BRACE;
                default: return KEY_NOTYPE;
        }
}