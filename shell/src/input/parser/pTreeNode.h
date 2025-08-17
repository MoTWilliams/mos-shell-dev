#ifndef P_TREE_NODE_H
#define P_TREE_NODE_H

#include "tokenNode.h"

struct PTNode;
typedef struct PTNode PTNode;

typedef enum {
        PTNODE_CHUNK,
        PTNODE_OP,
        PTNODE_CMD
} PTNodeType;

typedef enum {
        CMD_NOTYPE,
        CMD_SIMPLE,
        CMD_IF,
        CMD_LOOP,
        CMD_CASE,
        CMD_SUBSHELL,
        CMD_GROUP
} CommandType;

struct PTNode {
        /* Token(s) */
        Token* first;
        Token* last;

        /* Tree link(s) */
        PTNode* lhs;
        PTNode* rhs;

        /* Metadata */
        PTNodeType ptnType;
        CommandType cType;
};

/* Methods */
PTNode* ptNode_create(Token* curTok, PTNodeType type);
void ptNode_destroy(PTNode* ptNode);

void ptNode_build(PTNode* ptNode, TokType delimiter);

#endif