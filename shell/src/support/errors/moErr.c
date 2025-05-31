#include "moErr.h"

#include "textStylesAndColors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REGISTER_ERR(errNo) sprintf(splash + len, "%s", #errNo + 4)

ErrNo report_err(Fatality fatal, ErrNo errNo, char* msg,
                        const char* file, int line) {
        /* The error type as a string--max 1028 characters */
        char splash[1028 + 1] = {'\0'};

        size_t len = 0;
        sprintf(splash, "%s", fatal ? "FATAL_" : "");
        len = strlen(splash); 

        /* This function was called in error or with ERR_NONE (0) */
        if (!errNo) {
                printf(FG_GRN "Not actually an error\n" RESET);
        }
        
        /* Add the error code to the [ERROR_SPLASH]*/
        REGISTER_ERR(errNo);

        /* Display the error splash and message */
        fprintf(stderr, (FG_RED "[" BOLD "%s" UNBOLD "] " RESET), splash);
        fprintf(stderr, "FILE: \"%s\"; LINE: %d; %s\n", file, line, msg);
        
        /* Exit the program immediately on fatal error */
        if (fatal) {
                exit(1);
        }

        /* Otherwise, just return the error number */
        return errNo;
}