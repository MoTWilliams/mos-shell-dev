/* moCalloc() unit tests */
#include "test_mem.h"
#include "mem.h"

#include "test_utility.h"

static Bool test_moCalloc_allocFailure_FATAL();
static Bool test_moCalloc_zeroAlloc_FATAL();
static Bool test_moCalloc_normalAlloc_FATAL();
static Bool test_moCalloc_largeAlloc_FATAL();
static Bool test_moCalloc_nullInit_FATAL();

static Bool test_moCalloc_allocFailure_NONFATAL();
static Bool test_moCalloc_zeroAlloc_NONFATAL();
static Bool test_moCalloc_normalAlloc_NONFATAL();
static Bool test_moCalloc_largeAlloc_NONFATAL();
static Bool test_moCalloc_nullInit_NONFATAL();

int test_moCalloc(int* tTests, int* gTests) {
        /* Group fail counter */
        int gFails = 0;

        /* NONFATAL failure mode */

        gFails += TEST_ASSERT(
                test_moCalloc_allocFailure_NONFATAL(),
                "test_moCalloc_allocFailure_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_zeroAlloc_NONFATAL(),
                "test_moCalloc_zeroAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_normalAlloc_NONFATAL(),
                "test_moCalloc_normalAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_largeAlloc_NONFATAL(),
                "test_moCalloc_largeAlloc_NONFATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_nullInit_NONFATAL(),
                "test_moCalloc_nullInit_NONFATAL() == TRUE"
        );

        /* FATAL failure mode */

        gFails += TEST_ASSERT(
                test_moCalloc_allocFailure_FATAL(),
                "test_moCalloc_allocFailure_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_zeroAlloc_FATAL(),
                "test_moCalloc_zeroAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_normalAlloc_FATAL(),
                "test_moCalloc_normalAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_largeAlloc_FATAL(),
                "test_moCalloc_largeAlloc_FATAL() == TRUE"
        );

        gFails += TEST_ASSERT(
                test_moCalloc_nullInit_FATAL(),
                "test_moCalloc_nullInit_FATAL() == TRUE"
        );

        return gFails;
}

/* Test that FATAL mode triggers crash on allocation failure */
static OpStatus allocFailure_FATAL_OPERATION();

static Bool test_moCalloc_allocFailure_FATAL() {
        return runTestInChildProcess(
                "test_moCalloc_allocFailure_FATAL()",
                allocFailure_FATAL_OPERATION, EXPECT_FAILURE
        );
}

static OpStatus allocFailure_FATAL_OPERATION() {
        /* Deliberately attempt to allocate too much memory */
        size_t page;
        char* ptr = moCalloc(SIZE_HUGE, sizeof(*ptr), FATAL);

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
static OpStatus allocFailure_NONFATAL_OPERATION();

static Bool test_moCalloc_allocFailure_NONFATAL() {
        return runTestInChildProcess(
                "test_moCalloc_allocFailure_NONFATAL()",
                allocFailure_NONFATAL_OPERATION, EXPECT_FAILURE
        );
}

static OpStatus allocFailure_NONFATAL_OPERATION() {
        /* Deliberately attempt to allocate too much memory */
        size_t page;
        char* ptr = moCalloc(SIZE_HUGE, sizeof(*ptr), NONFATAL);

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
static OpStatus zeroAlloc_FATAL_OPERATION();

static Bool test_moCalloc_zeroAlloc_FATAL() {
        return runTestInChildProcess(
                "test_moCalloc_zeroAlloc_FATAL()", 
                zeroAlloc_FATAL_OPERATION, EXPECT_SUCCESS
        );
}

static OpStatus zeroAlloc_FATAL_OPERATION() {
        /* Attempt to allocate size zero memory block */
        char* ptr = moCalloc(SIZE_ZERO, sizeof(*ptr), FATAL);

        /* Attempt to free the pointer--test is passed if this doesn't crash */
        moFree(ptr);

        /* If we get this far, test passed */
        return OP_SUCCESS;
}


/* Tests that NONFATAL mode creates a valid pointer on size zero allocation and 
 * that the pointer can safely be freed */
static OpStatus zeroAlloc_NONFATAL_OPERATION();

static Bool test_moCalloc_zeroAlloc_NONFATAL() {
        return runTestInChildProcess(
                "test_moCalloc_zeroAlloc_NONFATAL()", 
                zeroAlloc_NONFATAL_OPERATION, EXPECT_SUCCESS
        );
}

static OpStatus zeroAlloc_NONFATAL_OPERATION() {
        /* Attempt to allocate size zero memory block */
        char* ptr = moCalloc(SIZE_ZERO, sizeof(*ptr), NONFATAL);

        /* Fail the test prematurely if allocation fails */
        if (!ptr) return OP_FAILURE;

        /* Attempt to free the pointer--test is passed if this doesn't crash */
        moFree(ptr);

        /* If we get this far, test passed */
        return OP_SUCCESS;
}


static Bool test_moCalloc_normalAlloc_FATAL() {
        /* Fork a child process, because moCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_normalAlloc_FATAL(): fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate memory block. Fatal moMalloc() failure 
                 * will crash the process and fail the test */
                char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr), FATAL);

                /* Exit child process cleanly if freeing didn't crash */
                moFree(ptr);
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

static Bool test_moCalloc_normalAlloc_NONFATAL() {
        /* Attempt a normal-sized allocation */
        char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        moFree(ptr);
        ptr = NULL;
        return TRUE;
}

static Bool test_moCalloc_largeAlloc_FATAL() {
        /* Fork a child process, because moCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_largeAlloc_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate large memory block. Fatal moMalloc() 
                 * failure will crash the process and fail the test */
                char* ptr = moCalloc(SIZE_LARGE, sizeof(*ptr), FATAL);

                /* Exit child process cleanly if freeing didn't crash */
                moFree(ptr);
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

static Bool test_moCalloc_largeAlloc_NONFATAL() {
        /* Attempt a large-but-not-too-large allocation */
        char* ptr = moCalloc(SIZE_LARGE, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        moFree(ptr);
        ptr = NULL;
        return TRUE;
}

static Bool test_moCalloc_nullInit_FATAL() {
        /* Fork a child process, because moCalloc failure will crash */
        int status = -99;
        size_t pid = fork();
        Bool isAllNull = TRUE;
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_nullInit_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate memory block. Fatal moMalloc() failure 
                 * will crash the process and fail the test */
                char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr), FATAL);

                /* If any value is non-NULL, zero-initializing has failed */
                int i;
                for (i = 0; i < SIZE_NORMAL; i++) {
                        if (ptr[i]) {
                                isAllNull = FALSE;
                        }
                }

                /* Exit child process cleanly if moCalloc() didn't crash */
                moFree(ptr);
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

static Bool test_moCalloc_nullInit_NONFATAL() {
        /* Attempt a normal-sized allocation--we will rely on moCalloc's 
         * built-in safety check this time */
        char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr), NONFATAL);
        Bool isAllNull = TRUE;

        /* If any value is non-NULL, zero-initializing has failed */
        int i;
        for (i = 0; i < SIZE_NORMAL; i++) {
                if (ptr[i]) {
                        isAllNull = FALSE;
                }
        }

        /* cleanup the pointer */
        moFree(ptr);
        ptr = NULL;

        /* I'm checking the value of isAllNull and explicitly returning FALSE 
         * on test failure, just for consistency and semantic clarity. It would 
         * also work (but be less readable) to simply return isAllNull */
        if (!isAllNull) return FALSE;

        /* Otherwise, all elements were correctly null-initialized--test
         * passed */
        return TRUE;
}