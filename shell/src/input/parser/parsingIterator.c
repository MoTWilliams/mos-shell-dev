#include "parsingIterator.h"

#include "mem.h"
#include "handleErrors.h"

ParsingIterator* pIter_create(struct Token* curTok) {
        ParsingIterator* cursor = safeMalloc(sizeof(*cursor), FATAL);

        cursor->curTok = curTok;
        cursor->nestDepth = 0;

        return cursor;
}

void pIter_destroy(ParsingIterator* cursor) {
        cursor->nestDepth = 0;
        cursor->curTok = NULL;

        safeFree(cursor);
}
