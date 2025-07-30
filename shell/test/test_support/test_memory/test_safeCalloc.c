/* safeCalloc() unit tests */
#include "test_mem.h"
#include "mem.h"

#include "test_utility.h"

static Bool test_safeCalloc_allocFailure_FATAL(void);
static Bool test_safeCalloc_zeroAlloc_FATAL(void);
static Bool test_safeCalloc_normalAlloc_FATAL(void);
static Bool test_safeCalloc_largeAlloc_FATAL(void);
static Bool test_safeCalloc_nullInit_FATAL(void);

static Bool test_safeCalloc_allocFailure_NONFATAL(void);
static Bool test_safeCalloc_zeroAlloc_NONFATAL(void);
static Bool test_safeCalloc_normalAlloc_NONFATAL(void);
static Bool test_safeCalloc_largeAlloc_NONFATAL(void);
static Bool test_safeCalloc_nullInit_NONFATAL(void);

int test_safeCalloc(int* tTests, int* gTests) {
        /* Group fail counter */
        int gFails = 0;

        /* NONFATAL failure mode */

        gFails += TEST_ASSERT(
                test_safeCalloc_allocFailure_NONFATAL(),
                "test_safeCalloc_allocFailure_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_zeroAlloc_NONFATAL(),
                "test_safeCalloc_zeroAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_normalAlloc_NONFATAL(),
                "test_safeCalloc_normalAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_largeAlloc_NONFATAL(),
                "test_safeCalloc_largeAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_nullInit_NONFATAL(),
                "test_safeCalloc_nullInit_NONFATAL() == TRUE"
        );

        /* FATAL failure mode */

        gFails += TEST_ASSERT(
                test_safeCalloc_allocFailure_FATAL(),
                "test_safeCalloc_allocFailure_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_zeroAlloc_FATAL(),
                "test_safeCalloc_zeroAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_normalAlloc_FATAL(),
                "test_safeCalloc_normalAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_largeAlloc_FATAL(),
                "test_safeCalloc_largeAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_safeCalloc_nullInit_FATAL(),
                "test_safeCalloc_nullInit_FATAL() == TRUE"
        );

        return gFails;
}

/* Test that FATAL mode triggers crash on allocation failure */
static OpStatus allocFailure_FATAL_OPERATION(void);

static Bool test_safeCalloc_allocFailure_FATAL(void) {
        return runTestInChildProcess(
                "test_safeCalloc_allocFailure_FATAL()",
                allocFailure_FATAL_OPERATION, EXPECT_FAILURE
        );
}

static OpStatus allocFailure_FATAL_OPERATION(void) {
        /* Deliberately attempt to allocate too much memory */
        size_t page;
        char* ptr = safeCalloc(SIZE_HUGE, sizeof(*ptr), FATAL);

        /* Touch each page to force physical allocation. This crashes when 
         * space runs out. */
        for (page = 0; page < SIZE_HUGE + 1; page += SIZE_PAGE) {
                ((char*)ptr)[page] = 1;
        }

        /* If we made it this far, the operation failed to fail, so we return 
         * operation success--test failed */
        return OP_SUCCESS;
}

/* Tests that NONFATAL mode gracefully returns NULL on allocation failure */
static OpStatus allocFailure_NONFATAL_OPERATION(void);

static Bool test_safeCalloc_allocFailure_NONFATAL(void) {
        return runTestInChildProcess(
                "test_safeCalloc_allocFailure_NONFATAL()",
                allocFailure_NONFATAL_OPERATION, EXPECT_FAILURE
        );
}

static OpStatus allocFailure_NONFATAL_OPERATION(void) {
        /* Deliberately attempt to allocate too much memory */
        size_t page;
        char* ptr = safeCalloc(SIZE_HUGE, sizeof(*ptr), NONFATAL);

        /* Failed initial allocation also counts as successful operation fail */
        if (!ptr) return OP_FAILURE;

        /* Touch each page to force physical allocation. This crashes when 
         * space runs out */
        for (page = 0; page < SIZE_HUGE + 1; page += SIZE_PAGE) {
                ((char*)ptr)[page] = 1;
        }

        /* If we made it this far, the operation failed to fail, so we return 
         * operation success--test failed */
        return OP_SUCCESS;
}


/* Tests that FATAL mode doesn't crash on size zero allocation and creates a 
 * pointer that can be freed without crashing */
static OpStatus zeroAlloc_FATAL_OPERATION(void);

static Bool test_safeCalloc_zeroAlloc_FATAL(void) {
        return runTestInChildProcess(
                "test_safeCalloc_zeroAlloc_FATAL()", 
                zeroAlloc_FATAL_OPERATION, EXPECT_SUCCESS
        );
}

static OpStatus zeroAlloc_FATAL_OPERATION(void) {
        /* Attempt to allocate size zero memory block */
        char* ptr = safeCalloc(SIZE_ZERO, sizeof(*ptr), FATAL);

        /* Attempt to free the pointer--test is passed if this doesn't crash */
        safeFree(ptr);

        /* If we get this far, test passed */
        return OP_SUCCESS;
}


/* Tests that NONFATAL mode creates a valid pointer on size zero allocation and 
 * that the pointer can safely be freed */
static OpStatus zeroAlloc_NONFATAL_OPERATION(void);

static Bool test_safeCalloc_zeroAlloc_NONFATAL(void) {
        return runTestInChildProcess(
                "test_safeCalloc_zeroAlloc_NONFATAL()", 
                zeroAlloc_NONFATAL_OPERATION, EXPECT_SUCCESS
        );
}

static OpStatus zeroAlloc_NONFATAL_OPERATION(void) {
        /* Attempt to allocate size zero memory block */
        char* ptr = safeCalloc(SIZE_ZERO, sizeof(*ptr), NONFATAL);

        /* Fail the test prematurely if allocation fails */
        if (!ptr) return OP_FAILURE;

        /* Attempt to free the pointer--test is passed if this doesn't crash */
        safeFree(ptr);

        /* If we get this far, test passed */
        return OP_SUCCESS;
}


static Bool test_safeCalloc_normalAlloc_FATAL(void) {
        /* Fork a child process, because safeCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_safeCalloc_normalAlloc_FATAL(): fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate memory block. Fatal safeMalloc() failure 
                 * will crash the process and fail the test */
                char* ptr = safeCalloc(SIZE_NORMAL, sizeof(*ptr), FATAL);

                /* Exit child process cleanly if freeing didn't crash */
                safeFree(ptr);
                ptr = NULL;
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child crashes trying to allocate, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        return TRUE;
}

static Bool test_safeCalloc_normalAlloc_NONFATAL(void) {
        /* Attempt a normal-sized allocation */
        char* ptr = safeCalloc(SIZE_NORMAL, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        safeFree(ptr);
        ptr = NULL;
        return TRUE;
}

static Bool test_safeCalloc_largeAlloc_FATAL(void) {
        /* Fork a child process, because safeCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_safeCalloc_largeAlloc_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate large memory block. Fatal safeMalloc() 
                 * failure will crash the process and fail the test */
                char* ptr = safeCalloc(SIZE_LARGE, sizeof(*ptr), FATAL);

                /* Exit child process cleanly if freeing didn't crash */
                safeFree(ptr);
                ptr = NULL;
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child crashes trying to allocate, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        return TRUE;
}

static Bool test_safeCalloc_largeAlloc_NONFATAL(void) {
        /* Attempt a large-but-not-too-large allocation */
        char* ptr = safeCalloc(SIZE_LARGE, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        safeFree(ptr);
        ptr = NULL;
        return TRUE;
}

static Bool test_safeCalloc_nullInit_FATAL(void) {
        /* Fork a child process, because safeCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        Bool isAllNull = TRUE;
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_safeCalloc_nullInit_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate memory block. Fatal safeMalloc() failure 
                 * will crash the process and fail the test */
                char* ptr = safeCalloc(SIZE_NORMAL, sizeof(*ptr), FATAL);

                /* If any value is non-NULL, zero-initializing has failed */
                int i;
                for (i = 0; i < SIZE_NORMAL; i++) {
                        if (ptr[i]) {
                                isAllNull = FALSE;
                        }
                }

                /* Exit child process cleanly if safeCalloc() didn't crash */
                safeFree(ptr);
                ptr = NULL;
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child crashes trying to allocate, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

        /* If zero-initializing failed, test failed. I'm checking the value of 
         * isAllNull and explicitly returning FALSE on test failure, just for 
         * consistency and semantic clarity. It would also work (but be less 
         * readable) to simply return isAllNull*/
        if (!isAllNull) return FALSE;

        /* Otherwise, all elements were correctly null-initialized--test
         * passed */
        return TRUE;        
}

static Bool test_safeCalloc_nullInit_NONFATAL(void) {
        /* Attempt a normal-sized allocation--we will rely on safeCalloc's 
         * built-in safety check this time */
        char* ptr = safeCalloc(SIZE_NORMAL, sizeof(*ptr), NONFATAL);
        Bool isAllNull = TRUE;

        /* If any value is non-NULL, zero-initializing has failed */
        int i;
        for (i = 0; i < SIZE_NORMAL; i++) {
                if (ptr[i]) {
                        isAllNull = FALSE;
                }
        }

        /* cleanup the pointer */
        safeFree(ptr);
        ptr = NULL;

        /* I'm checking the value of isAllNull and explicitly returning FALSE 
         * on test failure, just for consistency and semantic clarity. It would 
         * also work (but be less readable) to simply return isAllNull */
        if (!isAllNull) return FALSE;

        /* Otherwise, all elements were correctly null-initialized--test
         * passed */
        return TRUE;
}