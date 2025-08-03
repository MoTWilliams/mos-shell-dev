#include "tokenList.h"
#include "token.h"

#include "mem.h"
#include "handleErrors.h"

#include <stdio.h>

TokList* toks_create(void) {
        /* Allocate space for the token list object */
        TokList* toks = safeMalloc(sizeof(*toks), FATAL);

        /* No metadata to initialize */

        /* Allocate space for the contents of the token list */
        toks->tokList = dList_create(FATAL);

        /* Return the initialized token list */
        return toks;
}

void toks_destroy(TokList* toks) {
        /* If toks is already free and NULL, attempting to destroy the list 
         * inside will cause a segfault, so we'll just return without doing 
         * anything instead */
        if (!toks) return;

        /* Free and reset the token list */
        dList_destroy(toks->tokList);

        /* No metadata to reset */

        /* Free token list object */
        safeFree(toks);
}

void toks_addEmptyToken(TokList* toks) {
        dList_append(toks->tokList, NODE_TOKEN, FATAL);
}

/* For debugging */
void toks_print(TokList* toks) {
        DLNode* current = NULL; /* Reference to the current token */

        /* Handle empty input */
        if (!toks || !toks->tokList || !toks->tokList->head) {
                printf("Input empty. No tokens.\n");
                return;
        }

        /* Initialize current to the first token */
        current = toks->tokList->head;

        /* Print the list of tokens */
        printf("Tokens: [");
        while (current) {
                token_print(current->data.token);
                if (current->next) {
                        printf(", ");
                }
                current = current->next;
        }
        printf("]\n\n");

        /* Print the line number of each token */
        current = toks->tokList->head;
        printf("Line numbers: [");
        while (current) {
                printf("%d", current->data.token->lineNo);

                if (current->next) {
                        printf(" ");
                }

                current = current->next;
        }
        printf("]\n\n");

        /* Print quote contexts */
        current = toks->tokList->head;
        printf("qTypes: [");
        while (current) {
                char* types = STR_TEXT(current->data.token->cqTypes, FATAL);

                printf("%s", types);

                if (current->next) {
                        printf(", ");
                }

                safeFree(types);

                current = current->next;
        }
        printf("]\n\n");

        /* Print token types */
        current = toks->tokList->head;
        printf("Token types: [");
        while (current) {
                switch (current->data.token->tType) {
                        case TOK_WORD: printf("TOK_WORD"); break;
                        case TOK_SEQ: printf("TOK_SEQ"); break;
                        case TOK_CASE_END: printf("TOK_CASE_END"); break;
                        case TOK_PIPE: printf("TOK_PIPE"); break;
                        case TOK_OR: printf("TOK_OR"); break;
                        case TOK_BG: printf("TOK_BG"); break;
                        case TOK_AND: printf("TOK_AND"); break;
                        case TOK_NOT: printf("TOK_NOT"); break;
                        case TOK_REDIR_IN: printf("TOK_REDIR_IN"); break;
                        case TOK_HEREDOC: printf("TOK_HEREDOC"); break;
                        case TOK_HERE_STRIP: printf("TOK_HERE_STRIP"); break;
                        case TOK_REDIR_OUT: printf("TOK_REDIR_OUT"); break;
                        case TOK_APPEND: printf("TOK_APPEND"); break;
                        default: printf("No token type"); break;
                }

                if (current->next) {
                        printf(", ");
                }
                
                current = current->next;
        }
        printf("]\n\n");

        /* Print keyword types */
        current = toks->tokList->head;
        printf("Keyword types: [");
        while (current) {
                switch (current->data.token->kType) {
                        /* If-statements */
                        case KEY_IF: printf("KEY_IF"); break;
                        case KEY_THEN: printf("KEY_THEN"); break;
                        case KEY_ELSE: printf("KEY_ELSE"); break;
                        case KEY_ELIF: printf("KEY_ELIF"); break;
                        case KEY_FI: printf("KEY_FI"); break;

                        /* Loops */
                        case KEY_FOR: printf("KEY_FOR"); break;
                        case KEY_IN: printf("KEY_IN"); break;
                        case KEY_WHILE: printf("KEY_WHILE"); break;
                        case KEY_UNTIL: printf("KEY_UNTIL"); break;
                        case KEY_DO: printf("KEY_DO"); break;
                        case KEY_DONE: printf("KEY_DONE"); break;

                        /* Switch/case statements */
                        case KEY_CASE: printf("KEY_CASE"); break;
                        case KEY_ESAC: printf("KEY_ESAC"); break;

                        /* Subshells and curly braces */
                        case KEY_L_PAREN: printf("KEY_L_PAREN"); break;
                        case KEY_R_PAREN: printf("KEY_R_PAREN"); break;
                        case KEY_L_BRACE: printf("KEY_L_BRACE"); break;
                        case KEY_R_BRACE: printf("KEY_R_BRACE"); break; 

                        /* Not a keyword or error */
                        default: printf("Not a keyword"); break;
                }

                if (current->next) {
                        printf(", ");
                }

                current = current->next;
        }
        printf("]\n");

        /* Say something, if the line ends with an unterminated quote string */
        if (TOKS_TAIL(toks)->untermQ) {
                REPORT_ERR(
                        NONFATAL, ERR_INPUT,
                        "IN toks_print(); Unterminated quote"
                );
                printf("\n");
        }
        
}