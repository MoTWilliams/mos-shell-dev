/* safeMalloc() unit tests */
#include "test_mem.h"
#include "mem.h"

Bool test_safeMalloc_allocFailure_FATAL(void);
Bool test_safeMalloc_normalAlloc_FATAL(void);

Bool test_safeMalloc_zeroAlloc_NONFATAL(void);

int test_safeMalloc(int* tTests, int* gTests) {
        /* Group fail counter */
        int gFails = 0;

        /* NONFATAL failure mode */

        gFails += TEST_ASSERT(
                test_safeMalloc_zeroAlloc_NONFATAL(),
                "test_safeMalloc_zeroAlloc_NONFATAL() == TRUE"
        );

        /* FATAL failure mode */

        gFails += TEST_ASSERT(
                test_safeMalloc_allocFailure_FATAL(),
                "test_safeMalloc_allocFailure_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeMalloc_normalAlloc_FATAL(),
                "test_safeMalloc_normalAlloc_FATAL() == TRUE"
        );

        return gFails;
}

Bool test_safeMalloc_allocFailure_FATAL(void) {
        printf("UNDER CONSTRUCTION ");
        return FALSE;
}

Bool test_safeMalloc_normalAlloc_FATAL(void) {
        printf("UNDER CONSTRUCTION ");
        return FALSE;
}

Bool test_safeMalloc_zeroAlloc_NONFATAL(void) {
        printf("UNDER CONSTRUCTION ");
        return FALSE;
}