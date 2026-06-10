/*
** c05_ex01 -- ft_recursive_factorial: int ft_recursive_factorial(int nb);
** Returns nb! computed recursively. nb < 0 is invalid -> 0. 0! = 1. Overflow is
** undefined, so only nb in 0..12 is asserted (12! fits an int, 13! does not).
*/

#include "runner.h"
#include <limits.h>

extern int	ft_recursive_factorial(int nb);

/* Reference factorial; negative -> 0. Only called with nb <= 12 (no overflow). */
static int	refFactorial(int nb)
{
    int	result;
    int	i;

    if (nb < 0)
        return (0);
    result = 1;
    i = 1;
    while (i <= nb)
    {
        result *= i;
        i++;
    }
    return (result);
}

static void	assertFactorial(int nb)
{
    TEST_ASSERT_EQUAL_INT(refFactorial(nb), ft_recursive_factorial(nb));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* 0! is 1. */
static void	testFactorialZero(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_recursive_factorial(0));
}

/* 1! is 1. */
static void	testFactorialOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_recursive_factorial(1));
}

/* Documented known values. */
static void	testFactorialKnownValues(void)
{
    TEST_ASSERT_EQUAL_INT(2, ft_recursive_factorial(2));
    TEST_ASSERT_EQUAL_INT(6, ft_recursive_factorial(3));
    TEST_ASSERT_EQUAL_INT(24, ft_recursive_factorial(4));
    TEST_ASSERT_EQUAL_INT(120, ft_recursive_factorial(5));
    TEST_ASSERT_EQUAL_INT(720, ft_recursive_factorial(6));
    TEST_ASSERT_EQUAL_INT(5040, ft_recursive_factorial(7));
    TEST_ASSERT_EQUAL_INT(3628800, ft_recursive_factorial(10));
    TEST_ASSERT_EQUAL_INT(479001600, ft_recursive_factorial(12));
}

/* Every value across the full non-overflowing range matches the reference. */
static void	testFactorialAllValidRange(void)
{
    int	nb;

    nb = 0;
    while (nb <= 12)
    {
        assertFactorial(nb);
        nb++;
    }
}

/* A negative argument is invalid and returns 0. */
static void	testFactorialNegativeReturnsZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_recursive_factorial(-1));
    TEST_ASSERT_EQUAL_INT(0, ft_recursive_factorial(-5));
    TEST_ASSERT_EQUAL_INT(0, ft_recursive_factorial(-100));
    TEST_ASSERT_EQUAL_INT(0, ft_recursive_factorial(INT_MIN));
}

void	glorpRun(void)
{
    RUN_TEST(testFactorialZero);
    RUN_TEST(testFactorialOne);
    RUN_TEST(testFactorialKnownValues);
    RUN_TEST(testFactorialAllValidRange);
    RUN_TEST(testFactorialNegativeReturnsZero);
}
