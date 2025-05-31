#include "engine.h"
#include "input.h" /* For InputMode */

#include "moErr.h"
#include "textStylesAndColors.h"

#include <stdio.h>


int main(int argc, char* argv[]) {
        /* Default (i.e., no arguments) mode is interactive */
        InputMode mode = MODE_INTERACTIVE;

        /* Startup command only has one optional additional argument. more than 
         * 2 total arguments is too many. */
        if (argc > 2) {
                REPORT_ERR(
                        NONFATAL, ERR_INPUT,
                        "IN main(); too many arguments"
                );
                return 1;
        }

        /* Welcome message */
        printf(FG_BLU_B BOLD "\nWelcome to Mo's Shell!\n\n" RESET);

        /* If the optional argument is present, set mode to MODE_BATCH */
        if (argv[1]) {
                mode = MODE_BATCH;
        }

        /* Run the shell in the appropriate mode. argv[] is automatically 
         * NULL-terminated, so passing in the second argument is always safe. */
        runShell(mode, argv[1]);

        /* You'll only see this if all else goes how it should. */
        printf(RESET "\nGoodbye!\n\n");

        return 0;
}