#include "engine.h"
#include "lexer.h"
#include "token.h"

#include "moString.h"
#include "doublyLinkedList.h"
#include "ansi_codes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cmdPrompt();
void getInput(String* input);

int runShell_interactive() {
        String* input = NULL; // To store input from the command line
        while (1) {
                // Ensure the cycle begins with free and empty memory
                if (input) {
                        str_destroy(input);
                        input = NULL;
                }

                // Initialize the input buffer
                input = str_create();

                // Print the prompt
                cmdPrompt();

                // Read input from the command line
                getInput(input);

                char* inputStr = STR_TEXT(input);

                // PLACEHOLDER: Read back input
                printf("You entered: %s\n", inputStr);

                TokList* toks = input_lex(inputStr);

                // PLACEHOLDER: Exit on "exit"
                if (!strcmp(str_getText(input), "exit")) {
                        free(inputStr);
                        return 0;
                }

                printf("Tokens: [");
                DLNode* current = toks->tokList->head;
                while (current) {
                        token_print(current->data.token);
                        if (current->next) {
                                printf(", ");
                        }
                        current = current->next;
                }
                printf("]\n");

                free(inputStr);
        }

        // Final cleanup
        str_destroy(input);
        input = NULL;

        return 0;
}

// Show the command prompt
void cmdPrompt() {
        printf(FG_BLU_B "--SHELL--> " RESET);
        fflush(stdout);
}

// Read into the String
void getInput(String* input) {
        // Get characters from stdin. Stop at newline
        while (1) {
                str_append(input, NULL, -1);

                // Stop appending and trim newline
                if (STR_AT(input, STR_LEN(input) - 1) == '\n') {
                        STR_AT(input, STR_LEN(input) - 1) = '\0';
                        STR_LEN(input)--;
                        break;
                }
        }
}