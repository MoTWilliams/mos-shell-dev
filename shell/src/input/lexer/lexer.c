#include "lexer.h"
#include "token.h"
#include "moString.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void skip_comments(TokList* toks, char* input, int* pos);
void capture_symbols(TokList* toks, char* input, int* pos);
void capture_words(TokList* toks, char* input, int* pos);

TokList* input_lex(char* input) {
        // Create an empty list to hold the tokens
        TokList* toks = toks_create();

        // Split the input string into tokens
        int pos = 0;
        while (1) {
                // Exit the loop when the null-terminator or EOF is reached
                if (!input[pos] || input[pos] == EOF) {
                        return toks;
                }

                // Skip whitespace
                if (isspace(input[pos])) {
                        pos++;
                        continue;
                }

                // TEST: Add each non-space character to the token list as a 
                // separate token
                toks_addEmptyToken(toks);
                
                str_append(TOKS_TAIL(toks), input, pos);
                
                pos++;
        }

        return NULL;
}

void skip_comments(TokList* toks, char* input, int* pos) {
        // Detect # and skip the rest of the line
        while(1) {

        }
}

void capture_symbols(TokList* toks, char* input, int* pos) {}

/* captureWord() helpers -- might be able to combine cleanly... */
void captureWord_sngQuote(TokList* toks, char* input, int* pos);
void captureWord_dblQuote(TokList* toks, char* input, int* pos);
void captureWord_backQuote(TokList* toks, char* input, int* pos);

void capture_word(TokList* toks, char* input, int* pos) {}

void captureWord_sngQuote(TokList* toks, char* input, int* pos) {}

void captureWord_dblQuote(TokList* toks, char* input, int* pos) {}

void captureWord_backQuote(TokList* toks, char* input, int* pos) {}

/* TokList methods */

TokList* toks_create() {
        // Allocate space for the token list object
        TokList* toks = malloc(sizeof(TokList));

        // Exit immediately if allocation fails
        if (!toks) {
                perror("Memory allocation failed");
                exit(1);
        }

        // No metadata to initialize

        // Allocate space for the contents of the token list
        toks->tokList = dList_create();

        // Return the initialized token list
        return toks;
}

void toks_destroy(TokList* toks) {
        // Nothing to do if memory is already free and empty
        if (!toks) {
                return;
        }

        // Free and reset the token list
        dList_destroy(toks->tokList);

        // No metadata to reset

        // Free token list object
        free(toks);
}

void toks_addEmptyToken(TokList* toks) {
        dList_append(toks->tokList, NODE_TOKEN);
}
