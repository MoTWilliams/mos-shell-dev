#ifndef TEST_MEM_H
#define TEST_MEM_H

#include "moAssert.h"
#include "test_utility.h"

#include "moErr.h"

#include <stdio.h>
#include <limits.h>     /* For ULONG_MAX */

#define SIZE_PAGE       4096            /* 4 KiB (typical memory page size) */
#define SIZE_ZERO       0               /* For consistency and semantics */
#define SIZE_NORMAL     32              /* 32 bytes (tiny memory block) */
#define SIZE_LARGE      (1024 * 1024)   /* 1 MiB = 1,048,576 bytes */

/* Make tests portable to 32-bit systems */
#if ULONG_MAX > 0xFFFFFFFFUL
        /* For 64-bit systems */
        #define SIZE_HUGE ((size_t)1UL << 32) /* 4 GiB (4,294,967,296 bytes) */
#else
        /* To avoid overflow on 32-bit systems--max is 4GiB - 1byte */
        #define SIZE_HUGE ((size_t)UINT_MAX) /* 4 GiB - 1 byte */
#endif

int test_mem(int* tTests, int* gTests);

int test_safeCalloc(int* tTests, int* gTests);
int test_safeMalloc(int* tTests, int* gTests);
int test_safeRealloc(int* tTests, int* gTests);

#endif