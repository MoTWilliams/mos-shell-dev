#include "lexer.h"
#include "token.h"
#include "moString.h"
#include "mem.h"
#include "constants.h"  /* For Boolean values */
#include "report_status.h"

#include <stdio.h>      /* For EOF */
#include <string.h>
#include <ctype.h>

#define IN_QUOTE(qContext)      ((qContext) == Q_SINGLE || \
                                 (qContext) == Q_DOUBLE || \
                                 (qContext) == Q_CMDSUB)


void capture_symbolTok(TokList* toks, char* input, int* pos, Bool* inTok);
void capture_wordTok(TokList* toks, char* input, int* pos, Bool* inTok);

TokList* input_lex(char* input) {
        /* Create an empty list to hold the tokens */
        TokList* toks = toks_create();

        /* Cursor context */
        Bool inTok = FALSE;

        /* Split the input string into tokens */
        int pos = 0;

        while (input[pos] && input[pos] != EOF) {
                /* Skip whitespace */
                if (isspace(input[pos])) {
                        pos++;
                        continue;
                }

                /* Capture operators (;,|,||,&,&&,<,<<,>,>>) and other
                 * symbols  (!,(),[],{}) */
                if (strchr(";|&<>!()", input[pos])) {
                        capture_symbolTok(toks, input, &pos, &inTok);
                        continue;
                }

                /* Everything else is a word */
                capture_wordTok(toks, input, &pos, &inTok);
        }

        return toks;
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
                        /* ;; */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second ; */
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                /* Set the type */
                                TOKS_TAIL(toks)->tType = TOK_CASE_END;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just ; */
                        TOKS_TAIL(toks)->tType = TOK_SEQ;
                        break;
                case '|':
                        /* || */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second | */
                                token_appendChar(
                                        TOKS_TAIL(toks), input, 
                                        (*pos) + 1, Q_NONE
                                );
                                /* Set the type */
                                TOKS_TAIL(toks)->tType = TOK_OR;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just | */
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

                                /* Check for heredoc with tab stripping <<- */
                                if (input[(*pos)+2] && input[(*pos)+2]=='-') {
                                        token_appendChar(
                                                TOKS_TAIL(toks), input, 
                                                (*pos) + 2, Q_NONE
                                        );
                                        TOKS_TAIL(toks)->tType = TOK_HERE_STRIP;
                                        (*pos) += 2;
                                        break;
                                }

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

        /* No need to set kType--it is already initialized to KEY_NONE, and 
         * symbols are not keywords */

        /* Leave the token */
        (*pos)++;
        *inTok = FALSE;
}

/* Word token helpers */
QType wordTok_getCharQType(char* input, int pos);
void wordTok_captureRecursiveCmdSub(
        TokList* toks, char* input, int* pos, QType* qContext);
void wordTok_escape(
        TokList* toks,char* input,int* pos,Bool* inTok,QType qContext);
Keyword wordTok_setKType(Token* tok);

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

                /* Handle command substitution $(...) */
                if (qContext == Q_NONE && input[(*pos) + 1] &&
                                c == '$' && input[(*pos) + 1] == '(') {
                        wordTok_captureRecursiveCmdSub(
                                toks, input, pos, &qContext
                        );
                        continue;
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
                        wordTok_escape(toks,input,pos,inTok,qContext);
                        continue;
                }

                /* Append regular char to the token's string */
                token_appendChar(TOKS_TAIL(toks), input, *pos, qContext);
                (*pos)++;
        }

        /* Is the token a reserved word? */
        TOKS_TAIL(toks)->kType = wordTok_setKType(TOKS_TAIL(toks)); /* this line */

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
                        return Q_CMDSUB;
                default: /* Not a quotation mark */
                        return Q_NONE;
        }
}

void wordTok_captureRecursiveCmdSub(
                TokList* toks, char* input, int* pos, QType* qContext) {
        int pDepth = 1;         /* Parentheses depth counter */
       
        /* Set quote context */
        TOKS_TAIL(toks)->untermQ = TRUE;
        *qContext = Q_CMDSUB;

        /* Capture opening $( and advance */
        token_appendChar(TOKS_TAIL(toks), input, *pos, *qContext);
        token_appendChar(TOKS_TAIL(toks), input, (*pos) + 1, *qContext);
        (*pos) += 2;

        /* Capture the full command-sub string, accounting for nesting */
        while (pDepth != 0) {
                /* Exit the loop early if at end of string, line, or file */
                if (!input[*pos] || input[*pos] == '\n' || input[*pos] == EOF) {
                        *qContext = Q_NONE;
                        return;
                }

                /* Add the current char to the active token */
                token_appendChar(TOKS_TAIL(toks), input, *pos, *qContext);

                /* Add to the nesting count if another parentheses is opened */
                if (input[*pos] == '(') {
                        pDepth++;
                }

                /* Subtract when parentheses are closed */
                if (input[*pos] == ')') {
                        pDepth--;
                }

                /* Move on to the next char in input */
                (*pos)++;
        }

        /* Clean up context */
        TOKS_TAIL(toks)->untermQ = FALSE;
        *qContext = Q_NONE;
}

void wordTok_escape(
                TokList* toks,char* input,int* pos,Bool* inTok,QType qContext) {
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

        /* Advance down the input string accordingly */
        (*pos) += 2;
}

Keyword wordTok_setKType(Token* tok) {
        /* To avoid having to call free() repeatedly, we will just create the 
         * c-string from the token, set the value of this kType with a series 
         * of if-statements, destroy tht c-string, and return the set value. */
        Keyword kType = KEY_NOTYPE;
        char* tokStr = STR_TEXT(tok->tokText);

        /* Control flow: if-statements */
        if (!strcmp(tokStr, "if")) kType = KEY_IF;
        if (!strcmp(tokStr, "then")) kType = KEY_THEN;
        if (!strcmp(tokStr, "else")) kType = KEY_ELSE;
        if (!strcmp(tokStr, "elif")) kType = KEY_ELIF;
        if (!strcmp(tokStr, "fi")) kType = KEY_FI;

        /* Control flow: loops */
        if (!strcmp(tokStr, "for")) kType = KEY_FOR;
        if (!strcmp(tokStr, "in")) kType = KEY_IN;
        if (!strcmp(tokStr, "while")) kType = KEY_WHILE;
        if (!strcmp(tokStr, "until")) kType = KEY_UNTIL;
        if (!strcmp(tokStr, "do")) kType = KEY_DO;
        if (!strcmp(tokStr, "done")) kType = KEY_DONE;
        
        /* Switch/case statements */
        if (!strcmp(tokStr, "case")) kType = KEY_CASE;
        if (!strcmp(tokStr, "esac")) kType = KEY_ESAC;

        free(tokStr);
        return kType;
}

/* TokList methods */

TokList* toks_create(void) {
        /* Allocate space for the token list object */
        TokList* toks = moMalloc(sizeof(*toks));

        /* No metadata to initialize */

        /* Allocate space for the contents of the token list */
        toks->tokList = dList_create();

        /* Return the initialized token list */
        return toks;
}

void toks_destroy(TokList* toks) {
        /* If toks is already free and NULL, attempting to destroy the list 
         * inside will cause a segfault, so we'll just return without doing 
         * anything instead */
        if (!toks) {
                return;
        }

        /* Free and reset the token list */
        dList_destroy(toks->tokList);

        /* No metadata to reset */

        /* Free token list object */
        moFree(toks);
}

void toks_addEmptyToken(TokList* toks) {
        dList_append(toks->tokList, NODE_TOKEN);
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
        printf("]\n");

        /* Print quote contexts */
        current = toks->tokList->head;
        printf("qTypes: [");
        while (current) {
                char* types = STR_TEXT(current->data.token->cqTypes);

                printf("%s", types);

                if (current->next) {
                        printf(", ");
                }

                moFree(types);

                current = current->next;
        }
        printf("]\n");

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
                        case TOK_PAREN_L: printf("TOK_PAREN_L"); break;
                        case TOK_PAREN_R: printf("TOK_PAREN_R"); break;
                        default: printf("No token type"); break;
                }

                if (current->next) {
                        printf(", ");
                }
                
                current = current->next;
        }
        printf("]\n");

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
                msg_input_error("Unterminated quote");
        }
}