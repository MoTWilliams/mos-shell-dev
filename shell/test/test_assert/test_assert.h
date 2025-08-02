#ifndef MO_ASSERT_H
#define MO_ASSERT_H

#include "boolType.h"
#include "test_utility.h"

typedef enum {
    TEST_PASS,
    TEST_FAIL
} TestResult;

typedef enum {
        GRP_MEM
} TestGroup;

TestResult moAssert(
        Bool condition, const char* msg, const char* file, 
        int line, int* tTests, int* gTests);

#define TEST_ASSERT(condition, message) \
        (moAssert(condition, message, __FILE__, __LINE__, \
                tTests, gTests))

#endif