/* Chunk parse tree node methods */

#include "ptNodeTypes.h"
#include "tokenList.h"

#include "mem.h"

Chunk* chunk_create(Token* curTok) {
        Chunk* chunk = safeMalloc(sizeof(*chunk), FATAL);

        chunk->first = curTok;
        chunk->last = curTok;

        return chunk;
}

void chunk_destroy(Chunk* chunk) {
        chunk->first = NULL;
        chunk->last = NULL;

        safeFree(chunk);
}

Token* chunk_append(Chunk* chunk, Token* curTok) {
        chunk->last = curTok;

        return chunk->last;
}
