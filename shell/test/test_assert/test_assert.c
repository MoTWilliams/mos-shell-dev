#include "test_assert.h"

#include "textStylesAndColors.h"

#include <stdio.h>

TestResult moAssert(
        Bool condition, const char* msg, const char* file,
        int line, int* tTests, int* gTests) {
        /* Add 1 to the test count */
        (*tTests)++;
        (*gTests)++;

        if (!condition) {
                printf(FG_RED "[" BOLD "FAIL" UNBOLD "]-----");
                fflush(stdout);
                fprintf(stderr, 
                        "%s on LINE: %d of FILE: \"%s\"" RESET "\n",
                        msg, line, file
                );
                return TEST_FAIL;
        }

        printf(FG_GRN "[" BOLD "PASS" UNBOLD "]-----");
        fflush(stdout);
        printf("%s in FILE: \"%s\"" RESET "\n", msg, file);
        return TEST_PASS;
}