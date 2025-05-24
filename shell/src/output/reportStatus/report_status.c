#include "report_status.h"
#include "textStylesAndColors.h"

#include <stdio.h>

/* Non-fatal bad input (user error) */
void msg_input_error(char* message) {
        fprintf(stderr, 
                (FG_RED "[" BOLD "ERROR" UNBOLD "]" RESET " %s\n"),
                message
        );
}