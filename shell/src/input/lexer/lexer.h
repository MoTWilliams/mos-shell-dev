#ifndef LEXER_H
#define LEXER_H

#include "doublyLinkedList.h"

#define TOKS_TAIL(toks) ((toks)->tokList->tail->data.token)

typedef struct TokList {
        /* Contents */
        DList* tokList;

        /* Metadata */
} TokList;

TokList* toks_create(void);
void toks_destroy(TokList* toks);

/* Append an empty token object to the list */
void toks_addEmptyToken(TokList* toks);

void toks_print(TokList* toks);

TokList* input_lex(char* input);

#endif