#ifndef TOKEN_H
#define TOKEN_H

#include "constants.h"  // For Boolean values

struct String;
typedef struct String String;

typedef enum TokType {
        TOK_NOTYPE,     // 0
        TOK_WORD,       // 1
        TOK_SEQ,        // 2: ;
        TOK_PIPE,       // 3: |
        TOK_OR,         // 4: ||
        TOK_BG,         // 5: &
        TOK_AND,        // 6: &&
        TOK_NOT,        // 7: !
        TOK_REDIR_IN,   // 8: <
        TOK_HEREDOC,    // 9: <<
        TOK_REDIR_OUT,  // 10: >
        TOK_APPEND,     // 11: >>
        TOK_PAREN_L,    // 12: (
        TOK_PAREN_R,    // 13: )
} TokType;

typedef struct Token {
        // Contents
        String* tokText;

        // Metadata
        TokType tType;
        String* cqTypes; // A char denoting the context for each char in tokText
        Bool untermQ;
} Token;

Token* token_create();
void token_destroy(Token* tok);

void token_print(Token* tok);

#endif