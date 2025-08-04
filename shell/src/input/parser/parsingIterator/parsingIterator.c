#include "parsingIterator.h"
#include "tokenList.h"

#include "mem.h"

ParsingIterator* pIter_create(struct Token* tok) {
        ParsingIterator* cursor = safeMalloc(sizeof(*cursor), FATAL);

        cursor->curTok = tok;
        cursor->nestDepth = 0;

        return cursor;
}

void pIter_destroy(ParsingIterator* cursor) {
        cursor->curTok = NULL;
        cursor->nestDepth = 0;

        safeFree(cursor);
}