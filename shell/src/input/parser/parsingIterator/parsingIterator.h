#ifndef PARSING_ITERATOR_H
#define PARSING_ITERATOR_H

struct Token;

typedef struct ParsingIterator {
        struct Token* curTok;
        int nestDepth;
} ParsingIterator;

ParsingIterator* pIter_create(struct Token* tok);
void pIter_destroy(ParsingIterator* cursor);

#endif