/*
** c07_ex01 -- ft_range: int *ft_range(int min, int max);
** Returns a malloc'd array holding the consecutive values min, min+1, ...,
** max-1 (max is EXCLUDED), so its length is max - min. When min >= max the
** range is empty and the function returns NULL. The size is implicit (the caller
** knows max - min); the array is an independent, free-able heap block. Tested
** ranges stay modest so max - min never overflows int.
*/

#include "runner.h"
#include <stdlib.h>
#include <limits.h>

extern int	*ft_range(int min, int max);

/* For min < max: asserts a non-NULL array whose i-th element is min + i, then
** frees it. For min >= max: asserts NULL. */
static void	assertRange(int min, int max)
{
    int		*arr;
    int		size;
    int		i;

    arr = ft_range(min, max);
    if (min >= max)
    {
        TEST_ASSERT_NULL(arr);
        return ;
    }
    TEST_ASSERT_NOT_NULL(arr);
    size = max - min;
    i = 0;
    while (i < size)
    {
        TEST_ASSERT_EQUAL_INT(min + i, arr[i]);
        i++;
    }
    free(arr);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* min == max is an empty range -> NULL. */
static void	testRangeNullWhenMinEqualsMax(void)
{
    TEST_ASSERT_NULL(ft_range(5, 5));
    TEST_ASSERT_NULL(ft_range(0, 0));
    TEST_ASSERT_NULL(ft_range(-3, -3));
    TEST_ASSERT_NULL(ft_range(INT_MAX, INT_MAX));
    TEST_ASSERT_NULL(ft_range(INT_MIN, INT_MIN));
}

/* min > max -> NULL (value pairs chosen so max - min stays in range). */
static void	testRangeNullWhenMinGreaterThanMax(void)
{
    TEST_ASSERT_NULL(ft_range(11, 7));
    TEST_ASSERT_NULL(ft_range(5, -5));
    TEST_ASSERT_NULL(ft_range(0, -1));
    TEST_ASSERT_NULL(ft_range(INT_MAX, 0));
    TEST_ASSERT_NULL(ft_range(0, INT_MIN));
}

/* The documented example: max is excluded, so [7,11) is {7,8,9,10}. */
static void	testRangeKnownValues(void)
{
    int		*arr;

    arr = ft_range(7, 11);
    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    TEST_ASSERT_EQUAL_INT(8, arr[1]);
    TEST_ASSERT_EQUAL_INT(9, arr[2]);
    TEST_ASSERT_EQUAL_INT(10, arr[3]);
    free(arr);
}

/* A length-one range, including at both int extremes. */
static void	testRangeSingleElement(void)
{
    assertRange(5, 6);
    assertRange(0, 1);
    assertRange(-1, 0);
    assertRange(INT_MAX - 1, INT_MAX);
    assertRange(INT_MIN, INT_MIN + 1);
}

/* Positive ranges of various sizes. */
static void	testRangePositive(void)
{
    assertRange(0, 10);
    assertRange(1, 100);
    assertRange(50, 60);
}

/* Negative ranges and ranges spanning zero. */
static void	testRangeNegativeAndCrossingZero(void)
{
    assertRange(-5, 5);
    assertRange(-10, -5);
    assertRange(-100, -90);
    assertRange(-3, 3);
}

/* A larger range still fills correctly. */
static void	testRangeLargerRange(void)
{
    assertRange(0, 1000);
    assertRange(-500, 500);
}

/* Each call returns its own allocation. */
static void	testRangeReturnsFreshAllocations(void)
{
    int		*a;
    int		*b;

    a = ft_range(0, 5);
    b = ft_range(0, 5);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_TRUE(a != b);
    free(a);
    free(b);
}

/* Every min/max pair in a small grid: covers min < max, min == max, min > max. */
static void	testRangeSweep(void)
{
    int		min;
    int		max;

    min = -20;
    while (min <= 20)
    {
        max = -20;
        while (max <= 20)
        {
            assertRange(min, max);
            max++;
        }
        min++;
    }
}

/* Writes every element the allocation must hold across several sizes. With the
** test archive built under AddressSanitizer, an undersized malloc is caught here
** as a heap-buffer-overflow. */
static void	testRangeBufferIsFullyWritable(void)
{
    int		*arr;
    int		sizes[5];
    int		s;
    int		i;

    sizes[0] = 1;
    sizes[1] = 2;
    sizes[2] = 10;
    sizes[3] = 100;
    sizes[4] = 1000;
    s = 0;
    while (s < 5)
    {
        arr = ft_range(0, sizes[s]);
        TEST_ASSERT_NOT_NULL(arr);
        i = 0;
        while (i < sizes[s])
        {
            arr[i] = -1;
            i++;
        }
        free(arr);
        s++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testRangeNullWhenMinEqualsMax);
    RUN_TEST(testRangeNullWhenMinGreaterThanMax);
    RUN_TEST(testRangeKnownValues);
    RUN_TEST(testRangeSingleElement);
    RUN_TEST(testRangePositive);
    RUN_TEST(testRangeNegativeAndCrossingZero);
    RUN_TEST(testRangeLargerRange);
    RUN_TEST(testRangeReturnsFreshAllocations);
    RUN_TEST(testRangeSweep);
    RUN_TEST(testRangeBufferIsFullyWritable);
}
