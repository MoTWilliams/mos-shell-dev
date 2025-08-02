#include "test_utility.h"

#include "handleErrors.h"

#include <stdio.h>

Bool runTestInChildProcess(
                char* testName,OpStatus (*operation)(void),TestExpect toPass) {
        /* Fork a child process to avoid terminating the test suite on crash or 
         * successful intentional fatal failure */
        int status = -99;
        pid_t pid = fork();

        if ((int)pid < 0) {
                /* Build the fork error message */
                char errMsg[512 + 1];
                sprintf(errMsg, "IN %s; fork failed", testName);

                /* Report the fork error and prematurely fail the test */
                REPORT_ERR(NONFATAL, ERR_FORK, errMsg);
                return FALSE;
        } else if (pid == 0) {
                /* Run the test and store the result */
                OpStatus opSuccess = operation();

                /* If operation fails when it is expected to succeed, or if it 
                 * succeeds when it is expected to fail, exit in error */
                if ((toPass == EXPECT_SUCCESS && opSuccess == OP_FAILURE) ||
                    (toPass == EXPECT_FAILURE && opSuccess == OP_SUCCESS)) {
                        _exit(1);
                }

                /* Otherwise, exit cleanly */
                _exit(0);
        }

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        /* Successful operation passes the test */
        if (toPass == EXPECT_SUCCESS) {
                /* If the child crashes, test failed */
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return FALSE;

                /* Otherwise, test passed */
                return TRUE;
        }

        /* Operation failure passes the test */
        
        /* If the child exits cleanly, test failed */
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return FALSE;

        /* Otherwise, test passed */
        return TRUE;
}