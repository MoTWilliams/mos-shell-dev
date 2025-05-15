#include "engine.h"
#include "tokenizer.h"
#include "stringBuffer.h"
#include "doubleLinkedList.h"
#include "ansi_codes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cmdPrompt();
void getInput(StringBuffer* buff);

int runShell_interactive() {
        StringBuffer* input = NULL;    // To store input from the command line
        while (1) {
                // Ensure the cycle begins with free and empty memory
                if (input) {
                        strBuf_destroy(input);
                        input = NULL;
                }

                // Initialize the input buffer
                input = strBuf_create();

                // Print the prompt
                cmdPrompt();

                // Read input from the command line
                getInput(input);

                // Split the input into tokens
                DList* tokens = input_tokenize(input->strText);

                // PLACEHOLDER: Read back the input
                printf("You entered: %s\n", input->strText);

                // PLACEHOLDER: Print the tokens
                DListNode* current = tokens->head;

                printf("Tokens: [");

                while (current) {
                        printf("\"%s\"", current->data.token->tokText->strText);
                        current = current->next;

                        if (current) {
                                printf(", ");
                        }
                }
                
                printf("]\n");

                printf("Token types: [");

                current = tokens->head;
                while (current) {
                        printf("%d", current->data.token->type);
                        current = current->next;

                        if (current) {
                                printf(", ");
                        }
                }
                printf("]\n");

                dList_destroy(tokens);

                // PLACEHOLDER: Exit on exit
                if (!strcmp(input->strText, "exit")) {
                        strBuf_destroy(input);
                        exit(0);
                }
        }

        // Final cleanup
        strBuf_destroy(input);
        input = NULL;

        return 0;
}

void cmdPrompt() {
        printf(FG_BLU_B "--SHELL--> " RESET);
        fflush(stdout);
}

// Read into the buffer char-by-char until newline
void getInput(StringBuffer* buff) {
        char c;         // Store the char that was just appended
        
        // Get characters from stdin. Stop at newline
        while (1) {
                strBuf_append(buff, NULL, -1);

                // Stop appending and trim newline
                if (buff->strText[buff->length - 1] == '\n') {
                        buff->strText[buff->length - 1] = '\0';
                        buff->length--;
                        break;
                }
        }
}