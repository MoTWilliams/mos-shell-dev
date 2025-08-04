#ifndef PT_NODE_TYPES_H
#define PT_NODE_TYPES_H

struct Token;
struct DLNode;

/* OPERATOR */

typedef struct Operator {
        struct Token* opTok;
} Operator;

Operator* op_create(struct Token* curTok);
void op_destroy(Operator* op);

/* CHUNK */

typedef struct Chunk {
        struct Token* first;
        struct Token* last;
} Chunk;

Chunk* chunk_create(struct Token* curTok);
void chunk_destroy(Chunk* chunk);

struct Token* chunk_append(Chunk* chunk, struct Token* curTok);

Chunk* chunk_build(Chunk* chunk);

/* COMMAND */

typedef enum {
        CMD_SIMPLE,
        CMD_IF,
        CMD_LOOP,
        CMD_CASE,
        CMD_SUBSHELL,
        CMD_GROUP
} CmdType;

typedef struct Command {
        struct Token* first;
        struct Token* last;
        CmdType type;
} Command;

Command* command_create(struct Token* curTok);
void command_destroy(Command* cmd);

struct Token* command_append(Command* cmd, struct Token* curTok);

#endif