/*
** c11_ex00 -- ft_foreach: void ft_foreach(int *tab, int length, void (*f)(int));
** Applies the function f to each of the length elements of tab, in order.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_foreach(int *tab, int length, void (*f)(int));

#define MAX_RECORDED 32

static int	gRecorded[MAX_RECORDED];
static int	gCallCount;

/* Records every value it receives so call count and order can be checked. */
static void	recordValue(int n)
{
    if (gCallCount < MAX_RECORDED)
        gRecorded[gCallCount] = n;
    gCallCount++;
}

void	glorpUp(void)
{
    int	i;

    gCallCount = 0;
    i = 0;
    while (i < MAX_RECORDED)
    {
        gRecorded[i] = 0;
        i++;
    }
}

void	glorpDown(void)
{
}

/* f receives every element exactly once, in array order. */
static void	testForeachAppliesEachValueInOrder(void)
{
    int	tab[5] = {5, -3, 0, 42, 7};

    ft_foreach(tab, 5, recordValue);
    TEST_ASSERT_EQUAL_INT(5, gCallCount);
    TEST_ASSERT_EQUAL_INT_ARRAY(tab, gRecorded, 5);
}

/* Extreme int values are passed through unchanged. */
static void	testForeachPassesExtremeValues(void)
{
    int	tab[4] = {INT_MIN, -1, 1, INT_MAX};

    ft_foreach(tab, 4, recordValue);
    TEST_ASSERT_EQUAL_INT(4, gCallCount);
    TEST_ASSERT_EQUAL_INT_ARRAY(tab, gRecorded, 4);
}

/* A single element is visited exactly once. */
static void	testForeachSingleElement(void)
{
    int	tab[1] = {-99};

    ft_foreach(tab, 1, recordValue);
    TEST_ASSERT_EQUAL_INT(1, gCallCount);
    TEST_ASSERT_EQUAL_INT(-99, gRecorded[0]);
}

/* With length 0, f is never called. */
static void	testForeachLengthZeroNeverCallsF(void)
{
    int	tab[3] = {1, 2, 3};

    ft_foreach(tab, 0, recordValue);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
}

void	glorpRun(void)
{
    RUN_TEST(testForeachAppliesEachValueInOrder);
    RUN_TEST(testForeachPassesExtremeValues);
    RUN_TEST(testForeachSingleElement);
    RUN_TEST(testForeachLengthZeroNeverCallsF);
}
