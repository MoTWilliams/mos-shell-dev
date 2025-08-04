#include "input.h"
#include "tokenList.h"
#include "tokenNode.h"

#include "stringType.h"
#include "mem.h"
#include "boolType.h"  /* For Boolean values */

#include <stdio.h>      /* For EOF */
#include <string.h>
#include <ctype.h>

#define IN_QUOTE(qContext)      ((qContext) == Q_SINGLE || \
                                 (qContext) == Q_DOUBLE || \
                                 (qContext) == Q_CMDSUB)

static void handle_comments(char* input, int* pos, int* curLineNo);
static void tokenize_symbolTok(
        TokenList* tList, char* input, int* pos, Bool* inTok, int* curLineNo);
static void tokenize_wordTok(
        TokenList* tList, char* input, int* pos, Bool* inTok, int* curLineNo);

TokenList* input_lex(char* input) {
        TokenList* tList = tList_create();  /* Empty list to hold the tokens */
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
                if (strchr(";|&<>!(){}", input[pos])) {
                        tokenize_symbolTok(
                                tList, input, &pos, &inTok, &curLineNo
                        );
                        continue;
                }

                /* Everything else is a word */
                tokenize_wordTok(tList, input, &pos, &inTok, &curLineNo);
        }

        return tList;
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
                TokenList* tList, char* input, int* pos, 
                Bool* inTok, int* curLineNo) {
        /* Always start a new token, if any of these chars are detected */
        /* outside of a quoted string */
        tList_addEmptyToken(tList);
        *inTok = TRUE;

        /* Add the current char to the token */
        token_appendChar(tList->tail, input, *pos, Q_NOTYPE);

        /* Build special symbol tokens */
        switch (input[*pos]) {
                case ';':
                        /* ;; */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second ; */
                                token_appendChar(
                                        tList->tail, input, 
                                        (*pos) + 1, Q_NOTYPE
                                );
                                /* Set the type */
                                tList->tail->tType = TOK_CASE_END;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just ; */
                        tList->tail->tType = TOK_SEQ;
                        break;
                case '|':
                        /* || */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second | */
                                token_appendChar(
                                        tList->tail, input, 
                                        (*pos) + 1, Q_NOTYPE
                                );
                                /* Set the type */
                                tList->tail->tType = TOK_OR;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just | */
                        tList->tail->tType = TOK_PIPE;
                        break;
                case '&':
                        /* && */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second & */
                                token_appendChar(
                                        tList->tail, input, 
                                        (*pos) + 1, Q_NOTYPE
                                );
                                /* Set the type */
                                tList->tail->tType = TOK_AND;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just & */
                        tList->tail->tType = TOK_BG;
                        break;
                case '!':
                        tList->tail->tType = TOK_NOT;
                        break;
                case '<':
                        /* << */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second < */
                                token_appendChar(
                                        tList->tail, input, 
                                        (*pos) + 1, Q_NOTYPE
                                );

                                /* Check for heredoc with tab stripping <<- */
                                if (input[(*pos)+2] && input[(*pos)+2]=='-') {
                                        token_appendChar(
                                                tList->tail, input, 
                                                (*pos) + 2, Q_NOTYPE
                                        );
                                        tList->tail->tType = TOK_HERE_STRIP;
                                        (*pos) += 2;
                                        break;
                                }

                                /* Set the type */
                                tList->tail->tType = TOK_HEREDOC;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just < */
                        tList->tail->tType = TOK_REDIR_IN;
                        break;
                case '>':
                        /* >> */
                        if (input[(*pos)+1] && input[*pos] == input[(*pos)+1]) {
                                /* Append the second > */
                                token_appendChar(
                                        tList->tail, input, 
                                        (*pos) + 1, Q_NOTYPE
                                );
                                /* Set the type */
                                tList->tail->tType = TOK_APPEND;
                                (*pos)++;
                                break;
                        }
                        /* Otherwise, it's just > */
                        tList->tail->tType = TOK_REDIR_OUT;
                        break;
                /* SPECIAL CASES: These symbols are used to set off subshells
                 * and compound commands. Technically, they are word tokens
                 * with special keyword statuses. However, detecting these and
                 * setting their kTypes in tokenize_wordTok() would have been
                 * difficult and messy. We'll just do it here instead. */
                case '(':
                        tList->tail->tType = TOK_WORD;
                        tList->tail->kType = KEY_L_PAREN;
                        break;
                case ')':
                        tList->tail->tType = TOK_WORD;
                        tList->tail->kType = KEY_R_PAREN;
                        break;
                case '{':
                        tList->tail->tType = TOK_WORD;
                        tList->tail->kType = KEY_L_BRACE;
                        break;
                case '}':
                        tList->tail->tType = TOK_WORD;
                        tList->tail->kType = KEY_R_BRACE;
                        break;
                default:
                        break;
        }

        tList->tail->lineNo = *curLineNo;

        /* Leave the token */
        (*pos)++;
        *inTok = FALSE;
}

/* Word token helpers */
static QType wordTok_getCharQType(char* input, int pos);
/*static void wordTok_skipComment(
        char* input, int* pos, Bool* inTok, int* curLineNo);*/
static void wordTok_tokenizeRecursiveCmdSub(
        TokenList* tList, char* input, int* pos, QType* qContext);
static void wordTok_escape(
        TokenList* tList,char* input,int* pos,Bool* inTok,QType qContext);
static Keyword wordTok_setKType(Token* tok);

static void tokenize_wordTok(
                TokenList* tList, char* input, int* pos,
                Bool* inTok, int* curLineNo) {
        QType qContext = Q_NOTYPE;         /* Is cursor in a quote? */
        
        /* Start a new word token */
        tList_addEmptyToken(tList);
        tList->tail->tType = TOK_WORD;
        tList->tail->lineNo = *curLineNo;
        *inTok = TRUE;

        /* Loop to end of the input string */
        while (input[*pos] && input[*pos] != EOF) { 
                /* Store a reference to the current char in the input string 
                 * and its quote type */
                char c = input[*pos];
                QType charQ = wordTok_getCharQType(input, *pos);

                /* Exit the loop at the end of a word or a special character */
                if (qContext == Q_NOTYPE &&
                                (isspace(c) || strchr(";|&<>!(){}", c))) {
                        break;
                }

                /* Handle command substitution $(...) */
                if (qContext == Q_NOTYPE && input[(*pos) + 1] &&
                                c == '$' && input[(*pos) + 1] == '(') {
                        wordTok_tokenizeRecursiveCmdSub(
                                tList, input, pos, &qContext
                        );
                        continue;
                }

                /* Detect (and gather) open quotes */
                if (qContext == Q_NOTYPE && IN_QUOTE(charQ)) {
                        qContext = charQ;
                        tList->tail->untermQ = TRUE;
                        token_appendChar(tList->tail,input,*pos,qContext);
                        (*pos)++;
                        continue;
                }

                /* Detect and handle close quotes */
                if (IN_QUOTE(qContext) && charQ == qContext) {
                        token_appendChar(tList->tail,input,*pos,qContext);
                        qContext = Q_NOTYPE;
                        tList->tail->untermQ = FALSE;
                        (*pos)++;
                        continue;
                }

                /* Handle escape sequence */
                if (input[*pos] == '\\' && input[(*pos) + 1]) {
                        wordTok_escape(tList,input,pos,inTok,qContext);
                        continue;
                }

                /* Append regular char to the token's string */
                token_appendChar(tList->tail, input, *pos, qContext);
                (*pos)++;
        }

        /* Is the token a reserved word? */
        tList->tail->kType = wordTok_setKType(tList->tail);

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
                        return Q_NOTYPE;
        }
}

static void wordTok_tokenizeRecursiveCmdSub(
                TokenList* tList, char* input, int* pos, QType* qContext) {
        int pDepth = 1;         /* Parentheses depth counter */
       
        /* Set quote context */
        tList->tail->untermQ = TRUE;
        *qContext = Q_CMDSUB;

        /* tokenize opening $( and advance */
        token_appendChar(tList->tail, input, *pos, *qContext);
        token_appendChar(tList->tail, input, (*pos) + 1, *qContext);
        (*pos) += 2;

        /* tokenize the full command-sub string, accounting for nesting */
        while (pDepth != 0) {
                /* Exit the loop early if at end of string, line, or file */
                if (!input[*pos] || input[*pos] == '\n' || input[*pos] == EOF) {
                        *qContext = Q_NOTYPE;
                        return;
                }

                /* Add the current char to the active token */
                token_appendChar(tList->tail, input, *pos, *qContext);

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
        tList->tail->untermQ = FALSE;
        *qContext = Q_NOTYPE;
}

static void wordTok_escape(
                TokenList* tList,char* input,int* pos,Bool* inTok,QType qContext) {
        /* If not already in a token, create a new token */
        if (!(*inTok)) {
                tList_addEmptyToken(tList);
                tList->tail->tType = TOK_WORD;
                *inTok = TRUE;
        }

        /* Always gather the backslash and following char */
        token_appendChar(tList->tail, input, *pos, qContext);
        token_appendChar(tList->tail, input, (*pos) + 1, qContext);

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
        char* tokText = STR_TEXT(tok->text, FATAL);

        /* Control flow: if-statements */
        if (!strcmp(tokText, "if")) kType = KEY_IF;
        if (!strcmp(tokText, "then")) kType = KEY_THEN;
        if (!strcmp(tokText, "else")) kType = KEY_ELSE;
        if (!strcmp(tokText, "elif")) kType = KEY_ELIF;
        if (!strcmp(tokText, "fi")) kType = KEY_FI;

        /* Control flow: loops */
        if (!strcmp(tokText, "for")) kType = KEY_FOR;
        if (!strcmp(tokText, "in")) kType = KEY_IN;
        if (!strcmp(tokText, "while")) kType = KEY_WHILE;
        if (!strcmp(tokText, "until")) kType = KEY_UNTIL;
        if (!strcmp(tokText, "do")) kType = KEY_DO;
        if (!strcmp(tokText, "done")) kType = KEY_DONE;
        
        /* Switch/case statements */
        if (!strcmp(tokText, "case")) kType = KEY_CASE;
        if (!strcmp(tokText, "esac")) kType = KEY_ESAC;

        safeFree(tokText);
        return kType;
}