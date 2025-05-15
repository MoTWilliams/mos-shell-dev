#include "tokenizer.h"
#include "stringBuffer.h"
#include "doubleLinkedList.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void capture_operator(Token* tok, char* input, int* pos);

Token* token_create() {
        // Allocate space for the Token
        Token* token = malloc(sizeof(Token));

        if (!token) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the token's type 
        token->type = TOKEN_NOTYPE;

        // Initialize the contents with an empty StringBuffer
        token->tokText = strBuf_create();

        // Return the initialized Token
        return token;
}

void token_destroy(Token* token) {
        // Nothing to do if the memory is already free and empty
        if (!token) {
                return;
        }

        // Destroy the token's contents
        strBuf_destroy(token->tokText);

        // Destroy the token
        free(token);
}

DList* input_tokenize(char* input) {
        // Allocate space for the list of tokens
        DList* tokens = dList_create();

        // Flags to signify inside word or quoted string
        Bool inWord = FALSE;
        Bool inQuote = FALSE;

        // Traverse the string char-by-char to separate tokens
        for (int i = 0; input[i]; i++) {
                // Skip whitespace
                if (isspace(input[i])) {
                        continue;
                }

                // Capture operators (';', '|', '||', '<', '>', etc.). This 
                // will further increment the counter i if necessary
                if (strchr(";|<>&", input[i])) {
                        dList_append(tokens, NODE_TOKEN);
                        capture_operator(tokens->tail->data.token, input, &i);
                
                // For now, just skip everything that isn't an operator
                } else {
                        continue;
                }

                // IN PROGRESS: capture words and quoted strings. No need to 
                // figure out when to append. That's already done! Just edit 
                // the contents of tail.
        }

        // Return the populated token list
        return tokens;
}

void capture_operator(Token* tok, char* input, int* pos) {
        // Capture semicolon
        if (input[*pos] == ';') {
                strBuf_append(tok->tokText, input, *pos);
                tok->type = SEMICOLON;
                return;
        }
        
        // Capture two-character operators
        if (input[(*pos) + 1]) {
                if (input[*pos] == input[(*pos) + 1]) {
                        // Append the current and next characters to the token
                        strBuf_append(tok->tokText, input, *pos);
                        strBuf_append(tok->tokText, input, (*pos) + 1);
                        (*pos)++;
                        
                        // Set the token's type
                        switch (input[*pos]) {
                                case '|':
                                        tok->type = OR;
                                        return;
                                case '<':
                                        tok->type = HEREDOC;
                                        return;
                                case '>':
                                        tok->type = APPEND;
                                        return;
                                case '&':
                                        tok->type = AND;
                                        return;
                        }
                }
        }

        // Capture single-character operators

        // Append the character to the token
        strBuf_append(tok->tokText, input, *pos);

        // Set the token's type
        switch (input[*pos]) {
                case '|':
                        tok->type = PIPE;
                        return;
                case '<':
                        tok->type = REDIR_IN;
                        return;
                case '>':
                        tok->type = REDIR_OUT;
                        return;
                case '&':
                        tok->type = AND;
                        return;
        }
}
