#include "report_status.h"
#include "textStylesAndColors.h"
#include "engine.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
        /* Startup command only has one optional argument */
        if (argc > 2) {
                msg_input_error("too many arguments");
                return 1;
        }

        printf(FG_BLU_B BOLD "\nWelcome to Mo's Shell!\n\n" RESET);

        /* Start in batch mode */
        if (argv[1]) {
                printf(
                        "%s[--BATCH MODE UNDER CONSTRUCTION--]%s\n", 
                        BG_RED, NO_BGC
                );
        /* Start in interactive mode */
        } else {
                printf(FG_BLU_B "Entering interactive input mode..." RESET);
                printf("\n");

                runShell_interactive();

                printf(FG_BLU_B "Exiting interactive input mode..." RESET);
                printf("\n");
        }

        printf(RESET "\nGoodbye!\n\n");

        return 0;
}