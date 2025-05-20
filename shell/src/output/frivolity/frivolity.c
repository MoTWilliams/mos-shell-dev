/* Silly text flourishes */
#include "frivolity.h"
#include "ansi_codes.h"

#include <stdio.h>
#include <unistd.h> /* For usleep() */

/* Print three .'s, one at a time, in single color of choice */ 
void friv_animatedEllipses(char* color) {
        int i; /* Counter */
        
        fflush(stdout);
        for (int i = 0; i < 3; i++) {
                usleep(500000);
                printf("%s.%s", color, RESET);
                fflush(stdout);
        }
        usleep(500000);
}