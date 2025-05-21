#include "lexer.h"
#include "token.h"
#include "moString.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IN_QUOTE(qContext)      ((qContext) == Q_SINGLE || \
                                 (qContext) == Q_DOUBLE || \
                                 (qContext) == Q_BACKTICK)

void escape(TokList* toks,char* input,int* pos,Bool* inTok,QType qContext);
void skip_comment(TokList* toks, char* input, int* pos, Bool* inTok);
void capture_symbolTok(TokList* toks, char* input, int* pos, Bool* inTok);
void capture_wordTok(TokList* toks, char* input, int* pos, Bool* inTok);

TokList* input_lex(char* input) {
        /* Create an empty list to hold the tokens */
        TokList* toks = toks_create();

        /* Cursor context */
        Bool inTok = FALSE;

        /* Split the input string into tokens */
        int pos = 0;

        while (1) {
                /* Exit the loop when the null-terminator or EOF is reached */
                if (!input[pos] || input[pos] == EOF) {
                        return toks;
                }

                /* Skip whitespace */
                if (isspace(input[pos])) {
                        pos++;
                        continue;
                }

                /* Capture operators (;,|,||,&,&&,<,<<,>,>>) and other */
                /* symbols  (!,(),[],{}) */
                if (strchr(";|&<>!()", input[pos])) {
                        capture_symbolTok(toks, input, &pos, &inTok);
                        continue;
                }

                /* Everything else is a word */
                capture_wordTok(toks, input, &pos, &inTok);

                /* Exit the loop and return the tokens if the line ends with an 
                 * unterminated quoted string */
                if (TOKS_TAIL(toks)->untermQ) {
                        return toks;
                }

//                pos++;
        }

        return NULL;
}

void escape(TokList* toks,char* input,int* pos,Bool* inTok,QType qContext) {
        /* If not already in a token, create a new token */
        if (!(*inTok)) {
                toks_addEmptyToken(toks);
                TOKS_TAIL(toks)->tType = TOK_WORD;
                *inTok = TRUE;
        }

        /* Always gather the backslash and following char */
        token_appendChar(TOKS_TAIL(toks), input, *pos, qContext);
        token_appendChar(TOKS_TAIL(toks), input, (*pos) + 1, qContext);

        /* Even escaped, a single quote while inside a single-quoted string 
         * ends the token. */
        if (qContext == Q_SINGLE && input[(*pos) + 1] == '\'') {
                *inTok = FALSE;
        }

        // Advance down the input string accordingly
        (*pos) += 2;
}

void capture_symbolTok(TokList* toks, char* input, int* pos, Bool* inTok) {
        /* Always start a new token, if any of these chars are detected */
        /* outside of a quoted string */
        toks_addEmptyToken(toks);
        *inTok = TRUE;

        /* Add the current char to the token */
        token_appendChar(TOKS_TAIL(toks), input, *pos, Q_NONE);

        /* Build special symbol tokens */
        switch (input[*pos]) {
                case ';':
                        TOKS_TAIL(toks)->tType = TOK_SEQ;
                        break;
                case '|':
                        TOKS_TAIL(toks)->tType = TOK_PIPE;
                        break;
                case '&':
                        /* && */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second & */
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                /* Set the type */
                                TOKS_TAIL(toks)->tType = TOK_AND;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just & */
                        TOKS_TAIL(toks)->tType = TOK_BG;
                        break;
                case '!':
                        TOKS_TAIL(toks)->tType = TOK_NOT;
                        break;
                case '<':
                        /* << */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second < */
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                /* Set the type */
                                TOKS_TAIL(toks)->tType = TOK_HEREDOC;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just < */
                        TOKS_TAIL(toks)->tType = TOK_REDIR_IN;
                        break;
                case '>':
                        /* >> */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second > */
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                /* Set the type */
                                TOKS_TAIL(toks)->tType = TOK_APPEND;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just > */
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

        /* Leave the token */
        (*pos)++;
        *inTok = FALSE;
}

/* Word token helpers */
QType wordTok_getCharQType(char* input, int pos);

void capture_wordTok(TokList* toks, char* input, int* pos, Bool* inTok) {
        QType qContext = Q_NONE;         /* Is cursor in a quote? */
        
        /* Start a new word token */
        toks_addEmptyToken(toks);
        TOKS_TAIL(toks)->tType = TOK_WORD;
        *inTok = TRUE;

        /* Loop to end of the input string */
        while (input[*pos] && input[*pos] != EOF) {
                /* Store a reference to the current char in the input string 
                 * and its quote type */
                char c = input[*pos];
                QType charQ = wordTok_getCharQType(input, *pos);

                /* Exit the loop at the end of a word or a special character */
                if (qContext == Q_NONE &&
                                (isspace(c) || strchr(";|&<>!()", c))) {
                        break;
                }

                /* If non-escaped # is encountered, treat as a comment and skip 
                 * the rest of the line */
                if (qContext == Q_NONE && c == '#' &&
                                (*pos == 0 || input[(*pos) - 1] != '\\')) {
                        *inTok = FALSE;
                        break;
                }

                /* Detect (and gather) open quotes */
                if (qContext == Q_NONE && IN_QUOTE(charQ)) {
                        qContext = charQ;
                        TOKS_TAIL(toks)->untermQ = TRUE;
                        token_appendChar(TOKS_TAIL(toks),input,*pos,qContext);
                        (*pos)++;
                        continue;
                }

                /* Detect and handle close quotes */
                if (IN_QUOTE(qContext) && charQ == qContext) {
                        token_appendChar(TOKS_TAIL(toks),input,*pos,qContext);
                        qContext = Q_NONE;
                        TOKS_TAIL(toks)->untermQ = FALSE;
                        (*pos)++;
                        continue;
                }

                /* Handle escape sequence */
                if (input[*pos] == '\\' && input[(*pos) + 1]) {
                        escape(toks,input,pos,inTok,qContext);
                        continue;
                }

                /* Append regular char to the token's string */
                token_appendChar(TOKS_TAIL(toks), input, *pos, qContext);
                (*pos)++;
        }

        /* Move out of the token */
        *inTok = FALSE;
}

QType wordTok_getCharQType(char* input, int pos) {
        /* Set quote context */
        switch (input[pos]) {
                case '\'':
                        return Q_SINGLE;
                case '"':
                        return Q_DOUBLE;
                case '`':
                        return Q_BACKTICK;
                default: /* Not a quotation mark */
                        return Q_NONE;
        }
}

/* TokList methods */

TokList* toks_create(void) {
        /* Allocate space for the token list object */
        TokList* toks = malloc(sizeof(*toks));

        /* Exit immediately if allocation fails */
        if (!toks) {
                perror("Memory allocation failed");
                exit(1);
        }

        /* No metadata to initialize */

        /* Allocate space for the contents of the token list */
        toks->tokList = dList_create();

        /* Return the initialized token list */
        return toks;
}

void toks_destroy(TokList* toks) {
        /* Nothing to do if memory is already free and empty */
        if (!toks) {
                return;
        }

        /* Free and reset the token list */
        dList_destroy(toks->tokList);

        /* No metadata to reset */

        /* Free token list object */
        free(toks);
}

void toks_addEmptyToken(TokList* toks) {
        dList_append(toks->tokList, NODE_TOKEN);
}

void toks_print(TokList* toks) {
        /* Store a reference to the current token in the list. Initialize to 
         * the first token */
        DLNode* current = toks->tokList->head;

        /* Print the list of tokens */
        printf("Tokens: [");
        while (current) {
                token_print(current->data.token);
                if (current->next) {
                        printf(", ");
                }
                current = current->next;
        }
        printf("]\n");

        current = toks->tokList->head;
        printf("Quote Contexts: [");
        while (current) {
                char* types = STR_TEXT(current->data.token->cqTypes);

                printf("%s", types);

                if (current->next) {
                        printf(", ");
                }

                free(types);

                current = current->next;
        }
        printf("]\n");
}