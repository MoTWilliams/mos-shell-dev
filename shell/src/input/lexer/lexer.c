#include "input.h"
#include "tokenList.h"

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

static void handle_comments(char* input, int* pos, int* curLineNo);
static void tokenize_symbolTok(
        TokList* toks, char* input, int* pos, Bool* inTok, int* curLineNo);
static void tokenize_wordTok(
        TokList* toks, char* input, int* pos, Bool* inTok, int* curLineNo);

TokList* input_lex(char* input) {
        TokList* toks = toks_create();  /* Empty list to hold the tokens */
        Bool inTok = FALSE;             /* Cursor context */
        int pos = 0;                    /* Position in the input string */
        int curLineNo = 1;              /* Line counter */

        /* Split the input string into tokens */
        while (input[pos] && input[pos] != EOF) {
                /* Increment line count on un-escaped newline. This will only 
                 * fire in batch mode--newline signals end of input in 
                 * interactive mode */
                if (input[pos] == '\n') {
                        curLineNo++;
                        pos++;
                        continue;
                }
                
                /* Skip other whitespace entirely */
                if (isspace(input[pos])) {
                        pos++;
                        continue;
                }

                if (input[pos] == '#') {
                        handle_comments(input, &pos, &curLineNo);
                        continue;
                }

                /* Tokenize operators (;,|,||,&,&&,<,<<,>,>>) and other
                 * symbols  (!,(),[],{}) */
                if (strchr(";|&<>!()", input[pos])) {
                        tokenize_symbolTok(
                                toks, input, &pos, &inTok, &curLineNo
                        );
                        continue;
                }

                /* Everything else is a word */
                tokenize_wordTok(toks, input, &pos, &inTok, &curLineNo);
        }

        return toks;
}

static void handle_comments(char* input, int* pos, int* curLineNo) {
        char c = input[*pos];  /* Prime with the # */

        /* Skip characters until a newline or EOF is reached */
        while (c != '\n' && c != EOF) {
                /* Advance the cursor */
                (*pos)++;

                /* Look at the current char */
                c = input[*pos];
        }

        (*curLineNo)++;
        (*pos)++;
}

static void tokenize_symbolTok(
                TokList* toks, char* input, int* pos, 
                Bool* inTok, int* curLineNo) {
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

        TOKS_TAIL(toks)->lineNo = *curLineNo;

        /* Leave the token */
        (*pos)++;
        *inTok = FALSE;
}

/* Word token helpers */
static QType wordTok_getCharQType(char* input, int pos);
/*static void wordTok_skipComment(
        char* input, int* pos, Bool* inTok, int* curLineNo);*/
static void wordTok_tokenizeRecursiveCmdSub(
        TokList* toks, char* input, int* pos, QType* qContext);
static void wordTok_escape(
        TokList* toks,char* input,int* pos,Bool* inTok,QType qContext);
static Keyword wordTok_setKType(Token* tok);

static void tokenize_wordTok(
                TokList* toks, char* input, int* pos,
                Bool* inTok, int* curLineNo) {
        QType qContext = Q_NONE;         /* Is cursor in a quote? */
        
        /* Start a new word token */
        toks_addEmptyToken(toks);
        TOKS_TAIL(toks)->tType = TOK_WORD;
        TOKS_TAIL(toks)->lineNo = *curLineNo;
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

                /* Handle command substitution $(...) */
                if (qContext == Q_NONE && input[(*pos) + 1] &&
                                c == '$' && input[(*pos) + 1] == '(') {
                        wordTok_tokenizeRecursiveCmdSub(
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
        TOKS_TAIL(toks)->kType = wordTok_setKType(TOKS_TAIL(toks));

        /* Move out of the token */
        *inTok = FALSE;
}

static QType wordTok_getCharQType(char* input, int pos) {
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

static void wordTok_tokenizeRecursiveCmdSub(
                TokList* toks, char* input, int* pos, QType* qContext) {
        int pDepth = 1;         /* Parentheses depth counter */
       
        /* Set quote context */
        TOKS_TAIL(toks)->untermQ = TRUE;
        *qContext = Q_CMDSUB;

        /* tokenize opening $( and advance */
        token_appendChar(TOKS_TAIL(toks), input, *pos, *qContext);
        token_appendChar(TOKS_TAIL(toks), input, (*pos) + 1, *qContext);
        (*pos) += 2;

        /* tokenize the full command-sub string, accounting for nesting */
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

static void wordTok_escape(
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

static Keyword wordTok_setKType(Token* tok) {
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