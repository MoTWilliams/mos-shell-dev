#include "test_mem.h"
#include "mem.h"

#include <stdio.h>
#include <unistd.h>     /* For pid_t and fork() */
#include <sys/wait.h>   /* For waitpid() */
#include <limits.h>     /* For ULONG_MAX */

#define SIZE_PAGE       4096
#define SIZE_NORMAL     32
#define SIZE_LARGE      (1024 * 1024)

/* Make tests portable to 32-bit systems */
#if ULONG_MAX > 0xFFFFFFFFUL
    #define SIZE_HUGE ((size_t)1UL << 32)
#else
    #define SIZE_HUGE ((size_t)UINT_MAX)
#endif

/* moCalloc() tests */

Bool test_moCalloc_allocFailure();
Bool test_moCalloc_normalAlloc();
Bool test_moCalloc_largeAlloc();
Bool test_moCalloc_nullInit();

Bool test_moCalloc() {
        char* ptr = NULL;
        size_t pid;
        int status;
        
        /* Test normal allocation and zero-initialization */
        ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr));

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) {
                return FALSE;

        /* If any value is non-NULL, zero-initializing has failed */
        } else {
                int i;
                for (i = 0; i < SIZE_NORMAL; i++) {
                        if (ptr[i]) {
                                moFree(ptr);
                                return FALSE;
                        }
                }
        }
        moFree(ptr);
        ptr = NULL;

        /* Test large-but-not-too-large allocation (1 MB) */
        ptr = moCalloc(SIZE_LARGE, 1);
        if (!ptr) return FALSE;
        moFree(ptr);
        ptr = NULL;

        /* Test allocation failure--must spawn a child process, because failure 
         * triggers exit() */
        pid = fork();

        if ((int)pid < 0) {
                perror("fork failed");
                return FALSE;
        } else if (pid == 0) { /* Child process */
                size_t pg;
                ptr = moCalloc(SIZE_HUGE, 1);

                /* Touch each page to force real allocation */
                for (pg = 0; pg < SIZE_HUGE * 1; pg += SIZE_PAGE) {
                        ((char*)ptr)[pg] = 1;
                }

                fprintf(stderr, "(pid: %d) CHILD: Reached _exit(1)\n", getpid());
                
                /* Force test to fail if moCalloc does NOT fail as intended */
                _exit(1);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* If the child exits normally, exit on moCalloc() failure failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                return FALSE;
        }

        /* If we get this far, then all tests passed. */
        return TRUE;
}

Bool test_moCalloc_allocFailure() {
        /* Fork a child process, otherwise this test will terminate the test 
         * suite on success */
        size_t pid = fork();
        int status;
}

Bool test_moCalloc_normalAlloc() {
        char* ptr = moCalloc(SIZE_NORMAL, sizeof(*ptr));

        /* If ptr is still NULL after allocation, the allocation has failed */
        if (!ptr) {
                return FALSE;
        }

        /* Otherwise, allocation was successful--test passed */
        moFree(ptr);
        ptr = NULL;
        return TRUE;
}

Bool test_moCalloc_largeAlloc() {}

Bool test_moCalloc_nullInit() {

}



/* moMalloc() tests */

Bool test_moMalloc() {
        return TRUE;
}

Bool test_moRealloc() {
        return FALSE;
}