/*
** c05_ex04 -- ft_fibonacci: int ft_fibonacci(int index);
** Recursive Fibonacci: fib(0) = 0, fib(1) = 1, fib(n) = fib(n-1) + fib(n-2).
** index < 0 returns -1.
**
** The oracle computes fib iteratively (fast). The function under test is naive
** recursion (O(phi^n) calls), so the swept range is capped at 32 to stay quick;
** fib stays within int up to index 46, but evaluating those recursively is too
** slow to be worth it, and index 47+ overflows (undefined).
*/

#include "runner.h"
#include <limits.h>

extern int	ft_fibonacci(int index);

/* Iterative reference; index < 0 -> -1. */
static int	refFib(int index)
{
    int	a;
    int	b;
    int	t;
    int	i;

    if (index < 0)
        return (-1);
    if (index == 0)
        return (0);
    a = 0;
    b = 1;
    i = 1;
    while (i < index)
    {
        t = a + b;
        a = b;
        b = t;
        i++;
    }
    return (b);
}

static void	assertFib(int index)
{
    TEST_ASSERT_EQUAL_INT(refFib(index), ft_fibonacci(index));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The two base cases. */
static void	testFibBaseCases(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_fibonacci(0));
    TEST_ASSERT_EQUAL_INT(1, ft_fibonacci(1));
}

/* Documented known values. */
static void	testFibKnownValues(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_fibonacci(2));
    TEST_ASSERT_EQUAL_INT(2, ft_fibonacci(3));
    TEST_ASSERT_EQUAL_INT(3, ft_fibonacci(4));
    TEST_ASSERT_EQUAL_INT(5, ft_fibonacci(5));
    TEST_ASSERT_EQUAL_INT(8, ft_fibonacci(6));
    TEST_ASSERT_EQUAL_INT(13, ft_fibonacci(7));
    TEST_ASSERT_EQUAL_INT(144, ft_fibonacci(12));
    TEST_ASSERT_EQUAL_INT(6765, ft_fibonacci(20));
    TEST_ASSERT_EQUAL_INT(832040, ft_fibonacci(30));
}

/* Every index across 0..32 matches the iterative reference. */
static void	testFibSweep(void)
{
    int	index;

    index = 0;
    while (index <= 32)
    {
        assertFib(index);
        index++;
    }
}

/* A larger index still matches (kept modest so naive recursion stays fast). */
static void	testFibLargerIndex(void)
{
    TEST_ASSERT_EQUAL_INT(9227465, ft_fibonacci(35));
}

/* A negative index returns -1. */
static void	testFibNegativeReturnsMinusOne(void)
{
    TEST_ASSERT_EQUAL_INT(-1, ft_fibonacci(-1));
    TEST_ASSERT_EQUAL_INT(-1, ft_fibonacci(-5));
    TEST_ASSERT_EQUAL_INT(-1, ft_fibonacci(-100));
    TEST_ASSERT_EQUAL_INT(-1, ft_fibonacci(INT_MIN));
}

void	glorpRun(void)
{
    RUN_TEST(testFibBaseCases);
    RUN_TEST(testFibKnownValues);
    RUN_TEST(testFibSweep);
    RUN_TEST(testFibLargerIndex);
    RUN_TEST(testFibNegativeReturnsMinusOne);
}
