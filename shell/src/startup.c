#include "../headers/shell.h"
#include "../utility/ansi_codes.h"
#include "../utility/constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void startup_showPrompt();

int main() {
        /* The startup command from stdin */
        char* startupCmdBuff = calloc(BUFF_INIT + 1, sizeof(char));       

        while (1) {
                // Print the startup prompt
                startup_showPrompt();

                // Read startup command from stdin
                int c = '-';                    /* Next character from stdin */
                int i = 0;                      /* Counter */
                int capacity = BUFF_INIT;       /* Size of the string buffer */
                while (1) {
                        c = getchar();
                        if (c == '\n') {
                                break;
                        }

                        startupCmdBuff[i] = c;
                        i++;
                }
                startupCmdBuff[i] = '\0';

                printf("DEBUG: You entered '%s'\n", startupCmdBuff);

                if (!strcmp(startupCmdBuff, "exit")) {
                        break;
                }
        }

        return 0;
}

void startup_showPrompt() {
        printf(FG_BLU "[--" BOLD "Mo's Shell" UNBOLD "--]:" RESET);
        printf(" 'startup <optional/batch/file/path>' to open,");
        printf(" 'exit' to abort.\n> " );
        fflush(stdout);
}