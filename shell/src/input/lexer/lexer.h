#ifndef LEXER_H
#define LEXER_H

#define TOKS_TAIL(toks) ((toks)->tokList->tail->data.token->tokText)

struct DList;
typedef struct DList DList;

typedef struct TokList {
        // Contents
        DList* tokList;

        // Metadata
} TokList;

TokList* toks_create();
void toks_destroy(TokList* toks);

// Append an empty token object to the list
void toks_addEmptyToken(TokList* toks);

TokList* input_lex(char* input);

#endif