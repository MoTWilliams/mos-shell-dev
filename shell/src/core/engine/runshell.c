#include "engine.h"
#include "input.h"

#include "mem.h"
#include "textStylesAndColors.h"

#include <stdio.h>
#include <string.h>

static void cmdPrompt(void);

void runShell(InputMode mode, char* srcPath) {
        char* input = NULL; /* Store the input from stdin or file */
        
        /* Main program loop--once all input handling is written, it will all 
         * be abstracted into a handle_input() function */
        while (1) {
                /* Store the list of tokens */
                TokList* toks = NULL;

                /* Print the command prompt in interactive mode */
                if (mode == MODE_INTERACTIVE) {
                        cmdPrompt();
                }

                /* Build the input string */
                if (!(input = input_capture(mode, srcPath))) {
                        switch (mode) {
                                case MODE_INTERACTIVE:
                                        continue;
                                case MODE_BATCH:
                                        return;
                                default:
                                        exit(1);
                        }                        
                }

                /* Read back the input */
                printf("You entered: %s\n\n", input);

                /* PLACEHOLDER: Exit on input = "exit" */
                if (!strcmp(input, "exit")) {
                        break;
                }

                /* Split the input into tokens */
                toks = input_lex(input);

                /* DEBUG: Print the tokens and their metadata */
                toks_print(toks);

                /* loop cleanup */
                toks_destroy(toks);
                toks = NULL;
                moFree(input);
                input = NULL;

                /* Only run once if in batch input mode */
                if (mode == MODE_BATCH) {
                        break;
                }
        }

        /* final cleanup */
        moFree(input);
        input = NULL;
}

/* Show the command prompt */
static void cmdPrompt(void) {
        printf(FG_BLU_B "--SHELL--> " RESET);
        fflush(stdout);
}