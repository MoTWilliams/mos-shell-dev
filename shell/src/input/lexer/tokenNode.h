#ifndef TOKEN_NODE_H
#define TOKEN_NODE_H

#include "stringType.h"
#include "boolType.h"

struct Token;
typedef struct Token Token;

/* Token type labels */
typedef enum {
        TOK_WORD,       /* 0 */
        TOK_SEQ,        /* 1: ; (sequence) */
        TOK_CASE_END,   /* 2: ;; (used in switch/case statements) */
        TOK_PIPE,       /* 3: | */
        TOK_OR,         /* 4: || */
        TOK_BG,         /* 5: & (background) */
        TOK_AND,        /* 6: && */
        TOK_NOT,        /* 7: ! */
        TOK_REDIR_IN,   /* 8: < */
        TOK_HEREDOC,    /* 9: << */
        TOK_HERE_STRIP, /* 10: <<- (heredoc with tab stripping) */
        TOK_REDIR_OUT,  /* 11: > */
        TOK_APPEND      /* 12: >> */
} TokType;

/* Quote context labels */
typedef enum {
        Q_NOTYPE,         /* 0: - : not inside quotes */
        Q_SINGLE,       /* 1: S : '...' */
        Q_DOUBLE,       /* 2: D : "..." */
        Q_CMDSUB        /* 4: C : `...` and $(...) */
} QType;

/* Keyword/reserved word labels */
typedef enum {
        /* Not a keyword */
        KEY_NOTYPE,
        
        /* Control flow: if-statements */
        KEY_IF,
        KEY_THEN,
        KEY_ELSE,
        KEY_ELIF,
        KEY_FI,

        /* Control flow: loops */
        KEY_FOR,
        KEY_IN,
        KEY_WHILE,
        KEY_UNTIL,
        KEY_DO,
        KEY_DONE,

        /* Switch/case statements */
        KEY_CASE,
        KEY_ESAC,

        /* Subshells and command groups */
        KEY_L_PAREN,
        KEY_R_PAREN,
        KEY_L_BRACE,
        KEY_R_BRACE
} Keyword;

struct Token {
        /* Node data */
        String* text;

        /* List link */
        Token* next;

        /* Metadata */
        int lineNo; /* Used in batch input mode, mostly. In interactive mode, 
        * lineNo will only ever be 1. This could eventually become a Range 
        * struct, consisting of beginning and ending Position structs, which 
        * are, in turn, made of ints for line and column. */
        TokType tType;
        Keyword kType;
        String* cqTypes; /* Contexts for each char in tokText */
        Bool untermQ;
};

Token* token_create(void);
void token_destroy(Token* tok);

void token_appendChar(Token* tok, char* src, int pos, QType qType);
void token_print(Token* tok);

#endif