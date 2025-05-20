#include "token.h"
#include "moString.h"

#include <stdio.h>
#include <stdlib.h>

Token* token_create() {
        // ALlocate space for the token object
        Token* tok = malloc(sizeof(Token));

        // Exit immediately if allocation fails
        if (!tok) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize Metadata
        tok->tType = TOK_NOTYPE;
        tok->cqTypes = str_create();
        tok->untermQ = FALSE;

        // Allocate space for the token's contents
        tok->tokText = str_create();

        // Return the initialized token
        return tok;
}

void token_destroy(Token* tok) {
        // Nothing to do if memory is already free and empty
        if (!tok) {
                return;
        }

        // Free and reset the token's contents
        str_destroy(tok->tokText);
        tok->tokText = NULL;

        // Free and reset metadata
        tok->tType = TOK_NOTYPE;
        str_destroy(tok->cqTypes);
        tok->cqTypes = NULL;
        tok->untermQ = FALSE;

        // Free the token object
        free(tok);
}

void token_print(Token* tok) {
        char* tokTextPlain = STR_TEXT(tok->tokText);

        printf("%s", tokTextPlain);

        free(tokTextPlain);
}