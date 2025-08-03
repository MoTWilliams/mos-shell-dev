#ifndef P_TREE_NODE_H
#define P_TREE_NODE_H

/* FORWARD DECLARATIONS */

struct Token;

struct Operator;
struct Chunk;
struct Command;

/* Base node */
struct PTNode;
typedef struct PTNode PTNode;

typedef enum {
        PTNODE_CHUNK,
        PTNODE_OP,
        PTNODE_CMD
} PTNodeType;

typedef union {
        struct Operator* op;
        struct Chunk* chunk;
        struct Command* cmd;
} PTNodeData;

/* NODE DEFINITION */

struct PTNode {
        PTNodeType type;
        PTNodeData data;
        PTNode* lhs;
        PTNode* rhs;
};

/* NODE METHODS */
PTNode* ptNode_create(struct Token* curTok, PTNodeType type);
void ptNode_destroy(PTNode* node);

struct Token* ptNode_append(PTNode* node, struct Token* curTok);

#endif