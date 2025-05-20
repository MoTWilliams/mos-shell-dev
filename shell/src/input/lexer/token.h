#ifndef TOKEN_H
#define TOKEN_H

#include "constants.h"  /* For Boolean values */

struct String;
typedef struct String String;

typedef enum {
        TOK_NOTYPE,     /* 0 */
        TOK_WORD,       /* 1 */
        TOK_SEQ,        /* 2: ; (sequence) */
        TOK_PIPE,       /* 3: | */
        TOK_OR,         /* 4: || */
        TOK_BG,         /* 5: & (background) */
        TOK_AND,        /* 6: && */
        TOK_NOT,        /* 7: ! */
        TOK_REDIR_IN,   /* 8: < */
        TOK_HEREDOC,    /* 9: << */
        TOK_REDIR_OUT,  /* 10: > */
        TOK_APPEND,     /* 11: >> */
        TOK_PAREN_L,    /* 12: ( */
        TOK_PAREN_R,    /* 13: ) */
} TokType;

typedef enum {
        Q_NONE,         /* 0: - : not inside quotes */
        Q_SINGLE,       /* 1: S : inside ' ' */
        Q_DOUBLE,       /* 2: D : inside " " */
        Q_BACKTICK      /* 3: B : inside ` ` */
} QType;

typedef struct Token {
        /* Contents */
        String* tokText;

        /* Metadata */
        TokType tType;
        String* cqTypes; /* Contexts for each char in tokText */
        Bool untermQ;
} Token;

Token* token_create(void);
void token_destroy(Token* tok);

void token_appendChar(Token* tok, char* src, int pos, QType qType);
void token_print(Token* tok);

#endif