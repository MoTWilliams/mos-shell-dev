#include "test_mem.h"

int test_mem(int* tTests, int* gTests) {
        /* Start group counters at 0 */
        int gFails = 0;
        (*gTests) = 0;
        
        gFails += test_moCalloc(tTests, gTests);
        gFails += test_moMalloc(tTests, gTests);
        /* gFails += test_moMalloc(tTests, gTests); */

        return gFails;
}