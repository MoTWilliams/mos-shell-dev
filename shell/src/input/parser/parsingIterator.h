#ifndef PARSING_ITERATOR_H
#define PARSING_ITERATOR_H

struct ParsingIterator;
typedef struct ParsingIterator ParsingIterator;

struct Token;

struct ParsingIterator {
        struct Token* curTok;
        int nestDepth;
};

ParsingIterator* pIter_create(struct Token* curTok);
void pIter_destroy(ParsingIterator* cursor);

#endif