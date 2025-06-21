#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include "moBool.h"
#include <unistd.h>     /* For pid_t and fork() */
#include <sys/wait.h>   /* For waitpid() */

typedef enum {
        EXPECT_FAILURE,
        EXPECT_SUCCESS
} TestExpect;

typedef enum {
        OP_FAILURE,
        OP_SUCCESS
} OpStatus;

Bool runTestInChildProcess(
        char* testName, OpStatus (*operation)(void), TestExpect toPass);

#endif 