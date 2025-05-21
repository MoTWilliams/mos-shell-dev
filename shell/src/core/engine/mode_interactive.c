#include "engine.h"
#include "lexer.h"

#include "token.h"      /* I don't think I'll need this later either */

#include "moString.h"
#include "doublyLinkedList.h"   /* For the debug print statement. I don't think 
                                 * I'll need this later either...? */
#include "textStylesAndColors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void cmdPrompt(void);
static void getInput(String* input);

int runShell_interactive(void) {
        String* input = NULL; /* To store input from the command line */
        while (1) {
                /* Text from the command line, converted to a plain c-string */
                char* inputStr = NULL;

                /* The list of tokens */
                TokList* toks = NULL;

                /* Ensure the cycle begins with free and empty memory */
                if (input) {
                        str_destroy(input);
                        input = NULL;
                }

                /* Initialize the input buffer */
                input = str_create();

                /* Print the prompt */
                cmdPrompt();

                /* Read input from the command line */
                getInput(input);

                /* Get the plain c-string from the input String */
                inputStr = STR_TEXT(input);

                /* PLACEHOLDER: Read back input */
                printf("You entered: %s\n", inputStr);

                /* TODO: History expansion goes here (!! and !something) */

                /* Split the input string into tokens */
                toks = input_lex(inputStr);

                /* PLACEHOLDER: Exit on "exit" */
                if (!strcmp(inputStr, "exit")) {
                        free(inputStr);
                        str_destroy(input);
                        toks_destroy(toks);
                        return 0;
                }

                /* DEBUG: Print the tokens */
                toks_print(toks);

                /* Loop cleanup */
                free(inputStr);
                inputStr = NULL;
                toks_destroy(toks);
                toks = NULL;
        }

        /* Final cleanup */
        str_destroy(input);
        input = NULL;

        return 0;
}

/* Show the command prompt */
static void cmdPrompt(void) {
        printf(FG_BLU_B "--SHELL--> " RESET);
        fflush(stdout);
}

/* Read into the String */
static void getInput(String* input) {
        /* Get characters from stdin. Stop at newline */
        while (1) {
                str_append(input, NULL, -1);

                /* Stop appending and trim newline */
                if (STR_AT(input, STR_LEN(input) - 1) == '\n') {
                        STR_AT(input, STR_LEN(input) - 1) = '\0';
                        STR_LEN(input)--;
                        break;
                }
        }
}