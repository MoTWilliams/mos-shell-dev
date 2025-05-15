#include "tokenizer.h"
#include "stringBuffer.h"
#include "doubleLinkedList.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void capture_operator(DList* toks, char* input, int* pos);
void capture_sngQuote(DList* toks, char* input, int* pos);
void capture_dblQuote(DList* toks, char* input, int* pos);
void capture_word(DList* toks, char* input, int* pos);

Token* token_create() {
        // Allocate space for the Token
        Token* token = malloc(sizeof(Token));

        if (!token) {
                perror("Memory allocation failed");
                exit(1);
        }

        // Initialize the token's attributes
        token->type = TOKEN_NOTYPE;
        token->unterminatedQuote = FALSE;

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

        // Position counter
        int pos = 0;

        // Traverse the string char-by-char to separate tokens
        while (input[pos]) {
                // Skip whitespace
                if (isspace(input[pos])) {
                        pos++;
                        continue;
                }

                // Capture operators (';', '|', '||', '<', '>', etc.). This 
                // will further increment the counter i if necessary
                if (strchr(";|<>&", input[pos])) {
                        capture_operator(tokens, input, &pos);
                        continue;
                }

                // Capture single-quoted strings
                if (input[pos] == '\'') {
                        capture_sngQuote(tokens, input, &pos);
                        continue;
                }

                // Capture double-quoted strings
                if (input[pos] == '"') {
                        capture_dblQuote(tokens, input, &pos);
                        continue;
                }

                // Capture words
                capture_word(tokens, input, &pos);
        }

        // Return the populated token list
        return tokens;
}


void capture_operator(DList* toks, char* input, int* pos) {
        // Add the token to the tokens list
        dList_append(toks, NODE_TOKEN);
        
        // Capture two-character operators
        if (input[(*pos) + 1]) {
                if (input[*pos] == input[(*pos) + 1]) {
                        // Append the current and next characters to the token
                        strBuf_append(
                                toks->tail->data.token->tokText, 
                                input, *pos
                        );
                        strBuf_append(
                                toks->tail->data.token->tokText, 
                                input, (*pos) + 1
                        );
                        
                        // Set the token's type
                        switch (input[*pos]) {
                                case '|':
                                        toks->tail->data.token->type = OR;
                                        break;
                                case '<':
                                        toks->tail->data.token->type = HEREDOC;
                                        break;
                                case '>':
                                        toks->tail->data.token->type = APPEND;
                                        break;
                                case '&':
                                        toks->tail->data.token->type = AND;
                                        break;
                        }

                        // Increment the count outside the function
                        (*pos) += 2;
                        return;
                }
        }

        // Capture single-character operators

        // Append the character to the token
        strBuf_append(toks->tail->data.token->tokText, input, *pos);

        // Set the token's type
        switch (input[*pos]) {
                case ';':
                        toks->tail->data.token->type = SEMICOLON;
                        break;
                case '|':
                        toks->tail->data.token->type = PIPE;
                        break;
                case '<':
                        toks->tail->data.token->type = REDIR_IN;
                        break;
                case '>':
                        toks->tail->data.token->type = REDIR_OUT;
                        break;
                case '&':
                        toks->tail->data.token->type = AND;
                        break;
        }

        // Increment the count outside the function
        (*pos)++;
}

void capture_sngQuote(DList* toks, char* input, int* pos) {
        // Add a new token to the list
        dList_append(toks, NODE_TOKEN);

        // Set the token's type
        toks->tail->data.token->type = QUOTE_SINGLE;

        // Append the opening quote to the token
        strBuf_append(toks->tail->data.token->tokText, input, *pos);

        // Enter the quoted string
        toks->tail->data.token->unterminatedQuote = TRUE;
        
        // Gather characters
        while(1) {
                // Move on to the next character
                (*pos)++;

                // Exit the loop if the end of the string is reached
                if (!input[*pos]) {
                        break;
                }

                // Append the next character to the token
                strBuf_append(toks->tail->data.token->tokText, input, *pos);

                // Exit on the closing quote
                if (input[*pos] == '\'') {
                        toks->tail->data.token->unterminatedQuote = FALSE;
                        break;
                }
        }

        // Move out of the quoted string and reset inQuote, even if the line 
        // ends without terminating the quote
        (*pos)++;
}

void capture_dblQuote(DList* toks, char* input, int* pos) {
        // Add a new token to the list
        dList_append(toks, NODE_TOKEN);

        // Set the token's type
        toks->tail->data.token->type = QUOTE_DOUBLE;

        // Append the opening quote to the token
        strBuf_append(toks->tail->data.token->tokText, input, *pos);

        // Enter the quoted string
        toks->tail->data.token->unterminatedQuote = TRUE;
        
        // Gather characters
        while(1) {
                // Move on to the next character
                (*pos)++;

                // Exit the loop if the end of the string is reached
                if (!input[*pos]) {
                        break;
                }

                // Append the next character to the token
                strBuf_append(toks->tail->data.token->tokText, input, *pos);

                // Capture escaped double quotes
                if (input[(*pos) + 1]) {
                        if (input[*pos] == '\\' && input[(*pos) + 1] == '"') {
                                strBuf_append(
                                        toks->tail->data.token->tokText,
                                        input, (*pos) + 1
                                );
                                (*pos)++;
                                continue;
                        }
                }

                // Exit on the closing quote
                if (input[*pos] == '"') {
                        toks->tail->data.token->unterminatedQuote = FALSE;
                        break;
                }
        }

        // Move out of the quoted string and reset inQuote, even if the line 
        // ends without terminating the quote
        (*pos)++;
}

void capture_word(DList* toks, char* input, int* pos) {
        // Add a new token to the list
        dList_append(toks, NODE_TOKEN);

        // Set the token's type
        toks->tail->data.token->type = WORD;

        // Append the opening first character to the token
        strBuf_append(toks->tail->data.token->tokText, input, *pos);
        
        // Gather characters
        while(1) {
                // Move on to the next character
                (*pos)++;

                // Exit if a space, operator, single or double quote, or the 
                // end of the string is reached
                if (isspace(input[*pos]) || !input[*pos] 
                                        || strchr(";|<>&\'\"", input[*pos])) {
                        return;
                }

                // Append the next character to the token
                strBuf_append(toks->tail->data.token->tokText, input, *pos);
        }
}
