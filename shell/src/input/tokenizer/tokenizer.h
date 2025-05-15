#ifndef TOKENIZER_H
#define TOKENIZER_H

struct StringBuffer;
typedef struct StringBuffer StringBuffer;

struct DList;
typedef struct DList DList;

typedef enum TokenType {
        TOKEN_NOTYPE,
        WORD,
        QUOTE,
        SEMICOLON,
        PIPE,
        REDIR_IN,
        REDIR_OUT,
        BACKGROUND,
        OR,
        HEREDOC,
        APPEND,
        AND
} TokenType;

typedef struct Token {
        TokenType type;
        StringBuffer* tokText;
} Token;

Token* token_create();
void token_destroy(Token* token);

DList* input_tokenize(char* input);

#endif