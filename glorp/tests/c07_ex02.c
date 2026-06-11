/*
** c07_ex02 -- ft_ultimate_range: int ft_ultimate_range(int **range,
**                                                       int min, int max);
** Same array as ft_range (min..max-1, max excluded) but stored through *range;
** returns the size (max - min). When min >= max the range is empty: it returns 0
** and leaves *range untouched (the reference solutions do not NULL it), so that
** case checks only the return. (-1 on malloc failure is not exercised.) Tested
** ranges stay modest so max - min never overflows int.
*/

#include "runner.h"
#include <stdlib.h>
#include <limits.h>

extern int	ft_ultimate_range(int **range, int min, int max);

/* For min < max: asserts the return is max - min and *range[i] == min + i, then
** frees it. For min >= max: asserts the return is 0 (and leaves *range alone). */
static void	assertUltimateRange(int min, int max)
{
    int		*range;
    int		size;
    int		i;

    range = NULL;
    size = ft_ultimate_range(&range, min, max);
    if (min >= max)
    {
        TEST_ASSERT_EQUAL_INT(0, size);
        return ;
    }
    TEST_ASSERT_EQUAL_INT(max - min, size);
    TEST_ASSERT_NOT_NULL(range);
    i = 0;
    while (i < size)
    {
        TEST_ASSERT_EQUAL_INT(min + i, range[i]);
        i++;
    }
    free(range);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* min == max: empty range, returns 0. */
static void	testUltimateRangeZeroWhenMinEqualsMax(void)
{
    int		*range;

    range = NULL;
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 5, 5));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 0, 0));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, -3, -3));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, INT_MAX, INT_MAX));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, INT_MIN, INT_MIN));
}

/* min > max: returns 0 (value pairs chosen so max - min stays in range). */
static void	testUltimateRangeZeroWhenMinGreaterThanMax(void)
{
    int		*range;

    range = NULL;
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 11, 7));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 5, -5));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 0, -1));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, INT_MAX, 0));
    TEST_ASSERT_EQUAL_INT(0, ft_ultimate_range(&range, 0, INT_MIN));
}

/* The documented example: max excluded, [7,11) is {7,8,9,10}, size 4. */
static void	testUltimateRangeKnownValues(void)
{
    int		*range;
    int		size;

    range = NULL;
    size = ft_ultimate_range(&range, 7, 11);
    TEST_ASSERT_EQUAL_INT(4, size);
    TEST_ASSERT_NOT_NULL(range);
    TEST_ASSERT_EQUAL_INT(7, range[0]);
    TEST_ASSERT_EQUAL_INT(8, range[1]);
    TEST_ASSERT_EQUAL_INT(9, range[2]);
    TEST_ASSERT_EQUAL_INT(10, range[3]);
    free(range);
}

/* A length-one range, including at both int extremes. */
static void	testUltimateRangeSingleElement(void)
{
    assertUltimateRange(5, 6);
    assertUltimateRange(0, 1);
    assertUltimateRange(-1, 0);
    assertUltimateRange(INT_MAX - 1, INT_MAX);
    assertUltimateRange(INT_MIN, INT_MIN + 1);
}

/* Positive ranges of various sizes. */
static void	testUltimateRangePositive(void)
{
    assertUltimateRange(0, 10);
    assertUltimateRange(1, 100);
    assertUltimateRange(50, 60);
}

/* Negative ranges and ranges spanning zero. */
static void	testUltimateRangeNegativeAndCrossingZero(void)
{
    assertUltimateRange(-5, 5);
    assertUltimateRange(-10, -5);
    assertUltimateRange(-100, -90);
    assertUltimateRange(-3, 3);
}

/* A larger range still fills correctly. */
static void	testUltimateRangeLargerRange(void)
{
    assertUltimateRange(0, 1000);
    assertUltimateRange(-500, 500);
}

/* Each call writes its own allocation. */
static void	testUltimateRangeWritesDistinctBlocks(void)
{
    int		*a;
    int		*b;
    int		sa;
    int		sb;

    a = NULL;
    b = NULL;
    sa = ft_ultimate_range(&a, 0, 5);
    sb = ft_ultimate_range(&b, 0, 5);
    TEST_ASSERT_EQUAL_INT(5, sa);
    TEST_ASSERT_EQUAL_INT(5, sb);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_TRUE(a != b);
    free(a);
    free(b);
}

/* Every min/max pair in a small grid: covers min < max, min == max, min > max. */
static void	testUltimateRangeSweep(void)
{
    int		min;
    int		max;

    min = -20;
    while (min <= 20)
    {
        max = -20;
        while (max <= 20)
        {
            assertUltimateRange(min, max);
            max++;
        }
        min++;
    }
}

/* Writes every element the allocation must hold across several sizes. With the
** test archive built under AddressSanitizer, an undersized malloc is caught here
** as a heap-buffer-overflow. */
static void	testUltimateRangeBufferIsFullyWritable(void)
{
    int		*range;
    int		sizes[5];
    int		ret;
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
        range = NULL;
        ret = ft_ultimate_range(&range, 0, sizes[s]);
        TEST_ASSERT_EQUAL_INT(sizes[s], ret);
        TEST_ASSERT_NOT_NULL(range);
        i = 0;
        while (i < sizes[s])
        {
            range[i] = -1;
            i++;
        }
        free(range);
        s++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testUltimateRangeZeroWhenMinEqualsMax);
    RUN_TEST(testUltimateRangeZeroWhenMinGreaterThanMax);
    RUN_TEST(testUltimateRangeKnownValues);
    RUN_TEST(testUltimateRangeSingleElement);
    RUN_TEST(testUltimateRangePositive);
    RUN_TEST(testUltimateRangeNegativeAndCrossingZero);
    RUN_TEST(testUltimateRangeLargerRange);
    RUN_TEST(testUltimateRangeWritesDistinctBlocks);
    RUN_TEST(testUltimateRangeSweep);
    RUN_TEST(testUltimateRangeBufferIsFullyWritable);
}
