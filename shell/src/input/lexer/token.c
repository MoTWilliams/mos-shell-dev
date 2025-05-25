#include "token.h"
#include "moString.h"
#include "mem.h"

#include <stdio.h>
#include <stdlib.h>

Token* token_create(void) {
        /* ALlocate space for the token object */
        Token* tok = moMalloc(sizeof(*tok));

        /* Initialize Metadata */
        tok->tType = TOK_NOTYPE;
        tok->kType = KEY_NOTYPE;
        tok->cqTypes = str_create();
        tok->untermQ = FALSE;

        /* Allocate space for the token's contents */
        tok->tokText = str_create();

        /* Return the initialized token */
        return tok;
}

void token_destroy(Token* tok) {
        /* If tok is already free and NULL, attempting to destroy the strings 
         * inside will cause a segfault, so we'll just return without doing 
         * anything instead */
        if (!tok) {
                return;
        }

        /* Free and reset the token's contents */
        str_destroy(tok->tokText);
        tok->tokText = NULL;

        /* Free and reset metadata */
        tok->tType = TOK_NOTYPE;
        tok->kType = KEY_NOTYPE;
        str_destroy(tok->cqTypes);
        tok->cqTypes = NULL;
        tok->untermQ = FALSE;

        /* Free the token object */
        moFree(tok);
}

void token_appendChar(Token* tok, char* src, int pos, QType qType) {
        /* Add the character to the token's string */
        str_append(tok->tokText, src, pos);

        /* Add context marker to the character contexts string */
        switch (qType) {
                case Q_NONE: /* not in quotes */
                        str_append(tok->cqTypes, "-", 0);
                        return;
                case Q_SINGLE: /* '...' */
                        str_append(tok->cqTypes, "S", 0);
                        return;
                case Q_DOUBLE: /* "..." */
                        str_append(tok->cqTypes, "D", 0);
                        return;
                case Q_CMDSUB: /* `...` */
                        str_append(tok->cqTypes, "C", 0);
                        return;
        }
}

void token_print(Token* tok) {
        char* tokTextPlain = STR_TEXT(tok->tokText);

        printf("%s", tokTextPlain);

        moFree(tokTextPlain);
}