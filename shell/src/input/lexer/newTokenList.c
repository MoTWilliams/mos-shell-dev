#include "newTokenList.h"
#include "newTokenNode.h"

#include "mem.h"
#include "handleErrors.h"

#include <stdio.h>

TokenList* tList_create(void) {
        TokenList* tList = safeMalloc(sizeof(*tList), FATAL);

        tList->head = NULL;
        tList->tail = NULL;

        return tList;
}

void tList_destroy(TokenList* tList) {
        Token* curTok = NULL;

        if (!tList) return;

        curTok = tList->head;

        /* Destroy the list's nodes first */
        while (curTok) {
                Token* nexTok = curTok->next;

                token_destroy(curTok);

                curTok = nexTok;
        }

        safeFree(tList);
}

Token* tList_addEmptyToken(TokenList* tList) {
        Token* newTok = token_create();

        /* SPECIAL CASE: The list is currently empty--new node is now head */
        if (!tList->head) {
                tList->head = newTok;
                tList->tail = newTok;
                return newTok;
        }

        /* Otherwise, just append to the token list */
        tList->tail->next = newTok;
        tList->tail = newTok;

        return newTok;
}

void tList_print_DEBUG(TokenList* tList) {
        Token* curTok = NULL;

        /* Handle empty input */
        if (!tList || !tList->head) {
                printf("Input empty. No tokens.\n");
                return;
        }

        /* Print the tokens */
        curTok = tList->head;
        printf("Tokens: [");
        while (curTok) {
                token_print(curTok);
                if (curTok->next) printf(", ");
                curTok = curTok->next;
        }
        printf("]\n\n");

        /* Print the line number of each token */
        curTok = tList->head;
        printf("Line numbers: [");
        while (curTok) {
                printf("%d", curTok->lineNo);
                if (curTok->next) printf(" ");
                curTok = curTok->next;
        }
        printf("]\n\n");

        /* Print quote contexts */
        curTok = tList->head;
        printf("cqTypes: [");
        while (curTok) {
                char* types = STR_TEXT(curTok->cqTypes, FATAL);
                printf("%s", types);
                if (curTok->next) printf(", ");
                safeFree(types);
                curTok = curTok->next;
        }
        printf("]\n\n");

        /* Print token types */
        curTok = tList->head;
        printf("Token types: [");
        while (curTok) {
                switch (curTok->tType) {
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
                        default: printf("Not a token"); break;
                }
                if (curTok->next) printf(", ");
                curTok = curTok->next;
        }
        printf("]\n\n");

        /* Print keyword types */
        curTok = tList->head;
        printf("Keyword types: [");
        while (curTok) {
                switch (curTok->kType) {
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

                        /* Subshells and command groups */
                        case KEY_L_PAREN: printf("KEY_L_PAREN"); break;
                        case KEY_R_PAREN: printf("KEY_R_PAREN"); break;
                        case KEY_L_BRACE: printf("KEY_L_BRACE"); break;
                        case KEY_R_BRACE: printf("KEY_R_BRACE"); break;

                        /* Not a keyword or error */
                        default: printf("Not a keyword"); break;
                }
                if (curTok->next) printf(", ");
                curTok = curTok->next;
        }
        printf("]\n\n");

        /* Indicate unterminated quotes, if present */
        if (tList->tail->untermQ) {
                printf("Input contains unterminated quote.\n\n");
        }
}