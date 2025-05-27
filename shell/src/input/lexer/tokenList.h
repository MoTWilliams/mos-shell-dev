#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "token.h"
#include "doublyLinkedList.h"

#define TOKS_TAIL(toks) ((toks)->tokList->tail->data.token)
/* #define IS_LINE_END--if two adjacent tokens have unequal line numbers, the 
 * smaller one will be the end of a line */

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

#endif