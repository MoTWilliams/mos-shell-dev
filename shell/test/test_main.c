#include "moAssert.h"
#include "test_mem.h"

#include <stdio.h>

typedef enum {
        TESTS,
        FAILS
} GroupScores;

int main() {
        /* Tally tests and fails per group */
        int scores[1][2] = {0};

        /* Count up the failed tests */
        int tTests = 0;
        int tFails = 0;
        int gTests = 0;
        int gFails = 0;

        printf("\nRunning test on Mo's Shell\n\n");
        fflush(stdout);

        /* Test memory allocation */
        tFails += TEST_ASSERT(test_moCalloc(), "test_moCalloc() == TRUE");
        tFails += TEST_ASSERT(test_moMalloc(), "test_moMalloc() == TRUE");
        tFails += TEST_ASSERT(test_moRealloc(), "test_moRealloc() == TRUE");

        scores[GRP_MEM][TESTS] = gTests;
        scores[GRP_MEM][FAILS] = gFails;
        printf("Failed %d of %d memory tests\n\n", gFails, gTests);

        /* Reset counters for the next group */
        gTests = 0;
        gFails = 0;

        /* Summary */
        printf("SUMMARY:\n");
        printf("Failed %d of %d memory tests\n", 
                scores[GRP_MEM][FAILS], scores[GRP_MEM][TESTS]);
        printf("Failed %d of %d total tests\n\n", tFails, tTests);

        return 0;
}