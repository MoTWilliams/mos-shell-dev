#include "test_mem.h"

int test_mem(int* tTests, int* gTests) {
        /* Start group counters at 0 */
        int gFails = 0;
        (*gTests) = 0;
        
        gFails += test_safeCalloc(tTests, gTests);
        gFails += test_safeMalloc(tTests, gTests);
        /* gFails += test_safeRealloc(tTests, gTests); */

        return gFails;
}