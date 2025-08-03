#ifndef INPUT_H
#define INPUT_H

#include "tokenList.h"

/* ---Input capture and processing--- */

/* Capture the input from stdin (eventually raw terminal) or from a file */
typedef enum {
        MODE_INTERACTIVE,
        MODE_BATCH
} InputMode;

char* input_capture(InputMode mode, char* srcPath); 

/* Break the input into tokens */
struct TokList* input_lex(char* input);

/* Expand variables and substituted commands */
void input_expand(void);

/* Create the parse tree */
void input_parse(void);

#endif