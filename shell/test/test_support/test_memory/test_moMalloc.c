/* moMalloc() unit tests */
#include "test_mem.h"
#include "mem.h"

Bool test_moMalloc_allocFailure_FATAL();
Bool test_moMalloc_normalAlloc_FATAL();

Bool test_moMalloc_zeroAlloc_NONFATAL();

int test_moMalloc(int* tTests, int* gTests) {
        /* Group fail counter */
        int gFails = 0;

        /* NONFATAL failure mode */

        gFails += TEST_ASSERT(
                test_moMalloc_zeroAlloc_NONFATAL(),
                "test_moMalloc_zeroAlloc_NONFATAL() == TRUE"
        );

        /* FATAL failure mode */

        gFails += TEST_ASSERT(
                test_moMalloc_allocFailure_FATAL(),
                "test_moMalloc_allocFailure_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moMalloc_normalAlloc_FATAL(),
                "test_moMalloc_normalAlloc_FATAL() == TRUE"
        );

        return gFails;
}

Bool test_moMalloc_allocFailure_FATAL() {
        printf("UNDER CONSTRUCTION");
        return FALSE;
}

Bool test_moMalloc_normalAlloc_FATAL() {
        printf("UNDER CONSTRUCTION");
        return FALSE;
}

Bool test_moMalloc_zeroAlloc_NONFATAL() {
        printf("UNDER CONSTRUCTION");
        return FALSE;
}