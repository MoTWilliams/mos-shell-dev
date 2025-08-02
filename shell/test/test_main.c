#include "test_assert.h"
#include "test_mem.h"

#include <stdio.h>

#define NUM_GROUPS 1

typedef enum {
        TESTS,
        FAILS
} GroupScores;

int main(void) {
        /* Tally tests and fails per group */
        int scores[NUM_GROUPS][2] = {0};
        int group;

        /* Test and failure counters */
        int tTests = 0;
        int tFails = 0;
        int gTests = 0;

        printf("\nRunning test on Mo's Shell\n\n");
        fflush(stdout);

        /* Test memory allocation */
        scores[GRP_MEM][FAILS] = test_mem(&tTests, &gTests);
        scores[GRP_MEM][TESTS] = gTests;
        printf("Failed %d of %d memory tests\n\n", 
                scores[GRP_MEM][FAILS], scores[GRP_MEM][TESTS]
        );

        /* Summary */
        for (group = 0; group < NUM_GROUPS; group++) {
                tFails += scores[group][FAILS];
        }

        printf("SUMMARY:\n");
        printf("Failed %d of %d memory tests\n", 
                scores[GRP_MEM][FAILS], scores[GRP_MEM][TESTS]);
        printf("Failed %d of %d total tests\n\n", tFails, tTests);

        return 0;
}