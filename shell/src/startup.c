#include "report_status.h"
#include "frivolity.h"
#include "ansi_codes.h"
#include "shell.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
        // Startup command only has one optional argument
        if (argc > 2) {
                msg_input_error("too many arguments");
                return 1;
        }

        printf(FG_BLU_B BOLD "\nWelcome to Mo's Shell!\n\n" RESET);

        if (argv[1]) {
                printf(BG_RED "[--BATCH MODE UNDER CONSTRUCTION--]\n" RESET);
        } else {
                printf(FG_BLU_B "Entering interactive input mode" RESET);
                friv_animatedEllipses(FG_BLU_B);
                printf("\n");

                runShell_interactive();

                printf(FG_BLU_B "Exiting interactive input mode" RESET);
                friv_animatedEllipses(FG_BLU_B);
                printf("\n");
        }

        printf("Goodbye!\n\n");

        return 0;
}