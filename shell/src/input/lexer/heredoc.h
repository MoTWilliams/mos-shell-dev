#ifndef HEREDOC_H
#define HEREDOC_H

#include "stringType.h"
#include "boolType.h"

typedef enum {
        LEX_NORMAL,
        LEX_HEREDOC_FILL
} LexerMode;

struct PendingHeredoc;
typedef struct PendingHeredoc PendingHeredoc;

typedef struct HeredocQueue {
        PendingHeredoc* head;
        PendingHeredoc* tail;
} HeredocQueue;

struct PendingHeredoc {
        String* body;

        /* Metadata */
        Bool stripTabs;
        Bool quoted;
        int numLines;

        /* Link */
        PendingHeredoc* next;
};



#endif