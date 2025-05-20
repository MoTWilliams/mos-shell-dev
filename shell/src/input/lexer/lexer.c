#include "lexer.h"
#include "token.h"
#include "moString.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void handle_escape(TokList* toks, char* input, int* pos, Bool* inTok);
void skip_comment(TokList* toks, char* input, int* pos, Bool* inTok);

void capture_symbol(TokList* toks, char* input, int* pos, Bool* inTok);
void capture_words(TokList* toks, char* input, int* pos);

TokList* input_lex(char* input) {
        // Create an empty list to hold the tokens
        TokList* toks = toks_create();

        // Cursor context
        Bool inTok = FALSE;

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

                // Handle escape sequences outside of quoted strings: Current 
                // char is not the end of the string and is a backslash, and 
                // cursor is not inside a quoted string
                if (input[pos + 1] && input[pos] == '\\') {
                        handle_escape(toks, input, &pos, &inTok);
                        continue;
                }

                // Skip comments: When a non-escaped # is detected, advance to 
                // the end of the line without creating or adding to a token
                if (input[pos] == '#') {
                        skip_comment(toks, input, &pos, &inTok);
                        continue;
                }

                // Capture operators (;,|,||,&,&&,<,<<,>,>>) and other symbols 
                // (!,(),[],{})
                if (strchr(";|&<>!()[]{}", input[pos])) {
                        capture_symbol(toks, input, &pos, &inTok);
                        continue;
                }

                

                pos++;
        }

        return NULL;
}

void handle_escape(TokList* toks, char* input, int* pos, Bool* inTok) {
        // If not already in a token, create a new token
        if (!(*inTok)) {
                toks_addEmptyToken(toks);
                TOKS_TAIL(toks)->tType = TOK_WORD;
                *inTok = TRUE;
        }

        // Special case: Escaping a newline discards both the backslash and the 
        // newline--will use in batch mode
        if (input[(*pos) + 1] == '\n') {
                (*pos) += 2;
                return;
        }

        // Gather only the character following the backslash, not the backslash 
        // itself, and advance the cursor past both
        token_appendChar(TOKS_TAIL(toks), input, (*pos) + 1, Q_NONE);
        (*pos) += 2;
}

void skip_comment(TokList* toks, char* input, int* pos, Bool* inTok) {
        // If cursor is inside a token, leave the token--handles if a # is 
        // detected without a word-terminating character preceding
        if (*inTok) {
                *inTok = FALSE;
        }

        // Skip the rest of the line
        while(1) {
                // Exit loop at the end of the line, string, or file
                if (!input[*pos] || input[*pos]=='\n' || input[*pos]==EOF) {
                        return;
                }

                // Advance without gathering anything
                (*pos)++;
        }
}

void capture_symbol(TokList* toks, char* input, int* pos, Bool* inTok) {
        // Always start a new token, if any of these chars are detected outside 
        // of a quoted string
        toks_addEmptyToken(toks);
        *inTok = TRUE;

        // Add the current char to the token
        token_appendChar(TOKS_TAIL(toks), input, *pos, Q_NONE);

        // These aren't special tokens. They're just captured as words with 
        // only one character
        if (strchr("[]{}", input[*pos])) {
                TOKS_TAIL(toks)->tType = TOK_WORD;
        }

        // Build special symbol tokens
        switch (input[*pos]) {
                case ';':
                        TOKS_TAIL(toks)->tType = TOK_SEQ;
                        break;
                case '|':
                        TOKS_TAIL(toks)->tType = TOK_PIPE;
                        break;
                case '&':
                        // &&
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                // Append the second &
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                // Set the type
                                TOKS_TAIL(toks)->tType = TOK_AND;
                                (*pos)++;
                                break;
                        }
                        // Otherwise, it's just &
                        TOKS_TAIL(toks)->tType = TOK_BG;
                        break;
                case '!':
                        TOKS_TAIL(toks)->tType = TOK_NOT;
                        break;
                case '<':
                        // <<
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                // Append the second <
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                // Set the type
                                TOKS_TAIL(toks)->tType = TOK_HEREDOC;
                                (*pos)++;
                                break;
                        }
                        // Otherwise, it's just <
                        TOKS_TAIL(toks)->tType = TOK_REDIR_IN;
                        break;
                case '>':
                        // >>
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                // Append the second >
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                // Set the type
                                TOKS_TAIL(toks)->tType = TOK_APPEND;
                                (*pos)++;
                                break;
                        }
                        // Otherwise, it's just >
                        TOKS_TAIL(toks)->tType = TOK_REDIR_OUT;
                        break;
                case '(':
                        TOKS_TAIL(toks)->tType = TOK_PAREN_L;
                        break;
                case ')':
                        TOKS_TAIL(toks)->tType = TOK_PAREN_R;
                        break;
                default:
                        break;
        }

        // Leave the token
        (*pos)++;
        *inTok = FALSE;
}

void capture_word(TokList* toks, char* input, int* pos) {
        // Just use the QType enum for control flow *and* metadata
}

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
