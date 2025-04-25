#include "frivolity.h"
#include "ansi_codes.h"

#include <stdio.h>      // for printf()
#include <unistd.h>     // for usleep()

void friv_animatedEllipses(char* color) {
        fflush(stdout);
        for (int i = 0; i < 3; i++) {
                usleep(500000);
                printf("%s.%s", color, RESET);
                fflush(stdout);
        }
        usleep(500000);
}