#include "report_status.h"
#include "ansi_codes.h"

#include <stdio.h>      // For fprintf()
#include <errno.h>      // For standard error codes


void msg_input_error(char* message) {
        fprintf(stderr, 
                (FG_RED "[" BOLD "ERROR" UNBOLD "]" RESET " %s\n"),
                message
        );
}