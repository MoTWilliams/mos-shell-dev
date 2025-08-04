#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

struct Token;

typedef struct TokenList {
        struct Token* head;
        struct Token* tail;
} TokenList;

TokenList* tList_create(void);
void tList_destroy(TokenList* tList);

struct Token* tList_addEmptyToken(TokenList* tList);

void tList_print_DEBUG(TokenList* tList);

#endif