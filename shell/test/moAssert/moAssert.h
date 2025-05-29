#ifndef MO_ASSERT_H
#define MO_ASSERT_H

#include "moBool.h"

typedef enum {
    TEST_PASS,
    TEST_FAIL
} TestResult;

typedef enum {
        GRP_MEM
} TestGroup;

TestResult moAssert(
        Bool condition, const char* msg, const char* file, int line, 
        int* gFails, int* tTests, int* gTests);

#define TEST_ASSERT(condition, message) \
        (moAssert(condition, message, __FILE__, __LINE__, \
                &gFails, &tTests, &gTests))

#endif