/* moCalloc() unit tests */
#include "test_mem.h"
#include "mem.h"

Bool test_moCalloc_allocFailure_FATAL();
Bool test_moCalloc_zeroAlloc_FATAL();
Bool test_moCalloc_normalAlloc_FATAL();
Bool test_moCalloc_largeAlloc_FATAL();
Bool test_moCalloc_nullInit_FATAL();

Bool test_moCalloc_allocFailure_NONFATAL();
Bool test_moCalloc_zeroAlloc_NONFATAL();
Bool test_moCalloc_normalAlloc_NONFATAL();
Bool test_moCalloc_largeAlloc_NONFATAL();
Bool test_moCalloc_nullInit_NONFATAL();

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

Bool test_moCalloc_allocFailure_FATAL() {
        /* Fork a child process, otherwise successful allocation failure will 
         * terminate the test suite on success */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK, 
                        "IN test_moCalloc_allocFailure_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                size_t page = 0;
                char* ptr = moCalloc(SIZE_HUGE, sizeof(*ptr), FATAL);

                /* Touch each page to force real allocation. This crashes when 
                 * space runs out */
                for (page = 0; page < SIZE_HUGE + 1; page += SIZE_PAGE) {
                        ((char*)ptr)[page] = 1;
                }

                /* Force test to fail if moCalloc() does NOT fail as intended */
                _exit(1);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child exits normally instead of failing, the test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return FALSE;

        /* Otherwise, allocation failed as intended--test passed */
        return TRUE;
}

Bool test_moCalloc_allocFailure_NONFATAL() {
        /* Fork a child process, otherwise successful allocation failure will 
         * terminate the test suite on success */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_allocFailure_NONFATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                size_t page = 0;
                char* ptr = moCalloc(SIZE_HUGE, sizeof(*ptr), NONFATAL);

                /* If allocation fails as expected in NONFATAL mode, exit 
                 * cleanly */
                if (!ptr) _exit(0);

                /* Touch each page to force real allocation. This crashes when 
                 * space runs out */
                for (page = 0; page < SIZE_HUGE + 1; page += SIZE_PAGE) {
                        ((char*)ptr)[page] = 1;
                }

                /* Force test to fail if moCalloc() does NOT fail as intended */
                _exit(1);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child exits normally instead of failing, the test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return FALSE;

        /* Otherwise, allocation failed as intended--test passed */
        return TRUE;
}

Bool test_moCalloc_zeroAlloc_FATAL() {
        /* Fork a child process, because failure on freeing the pointer will 
         * result in crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_zeroAlloc_FATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate size zero memory block */
                char* ptr = moCalloc(SIZE_ZERO, sizeof(*ptr), FATAL);

                /* Check if the pointer can safely be freed */
                if (ptr) moFree(ptr);

                /* Exit child process cleanly if freeing didn't crash--if the 
                 * child crashes and exits in error, it will fail the test */
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child crashes trying to free ptr, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

        /* Otherwise, ptr was successfully freed, meaning zero-allocation was 
         * successful--test passed */
        return TRUE;
}

Bool test_moCalloc_zeroAlloc_NONFATAL() {
        /* Fork a child process, because failure on freeing the pointer will 
         * result in crash */
        int status = -99;
        size_t pid = fork();
        
        if ((int)pid < 0) {
                REPORT_ERR(
                        NONFATAL, ERR_FORK,
                        "IN test_moCalloc_zeroAlloc_NONFATAL(); fork failed"
                );
                return FALSE;
        } else if (pid == 0) {
                /* Attept to allocate size zero memory block */
                char* ptr = moCalloc(SIZE_ZERO, sizeof(*ptr), NONFATAL);

                /* Exit the process in error if moCalloc() fails */
                if (!ptr) {
                        REPORT_ERR(
                                NONFATAL, ERR_OUT_OF_MEMORY,
                                "Exiting test early due to moCalloc() failure"
                        );
                        _exit(1);
                
                /* Otherwise, check if the pointer can safely be freed */
                } else {
                        moFree(ptr);
                }

                /* Exit child process cleanly if freeing didn't crash--if the 
                 * child crashes and exits in error, it will fail the test */
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child crashes trying to free ptr, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

        /* Otherwise, ptr was successfully freed, meaning zero-allocation was 
         * successful--test passed */
        return TRUE;
}

Bool test_moCalloc_normalAlloc_FATAL() {
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

Bool test_moCalloc_normalAlloc_NONFATAL() {
        /* Attempt a normal-sized allocation */
        char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        moFree(ptr);
        ptr = NULL;
        return TRUE;
}

Bool test_moCalloc_largeAlloc_FATAL() {
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

Bool test_moCalloc_largeAlloc_NONFATAL() {
        /* Attempt a large-but-not-too-large allocation */
        char* ptr = moCalloc(SIZE_LARGE, sizeof(*ptr), NONFATAL);

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) return FALSE;

        /* Otherwise, allocation was successful--test passed */
        moFree(ptr);
        ptr = NULL;
        return TRUE;
}

Bool test_moCalloc_nullInit_FATAL() {
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

Bool test_moCalloc_nullInit_NONFATAL() {
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