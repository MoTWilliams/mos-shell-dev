#ifndef TOKEN_ITERATOR_H
#define TOKEN_ITERATOR_H

struct Token;

typedef struct TokenIterator {
        struct Token* current;
        int nestDepth;
} TokenIterator;

#endif