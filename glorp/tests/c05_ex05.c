/*
** c05_ex05 -- ft_sqrt: int ft_sqrt(int nb);
** Returns the integer square root of nb when nb is a perfect square, else 0.
** nb < 0 has no real root -> 0; 0 -> 0 and 1 -> 1. A non-perfect square (its
** real root is fractional, unrepresentable as int) -> 0.
** The largest perfect square that fits in int is 46340^2 = 2147395600
** (46341^2 overflows int), so that bounds the meaningful range; INT_MAX itself
** is not a perfect square. The oracle scans in long to avoid overflow.
*/

#include "runner.h"
#include <limits.h>

extern int	ft_sqrt(int nb);

/* Integer square root by linear scan; nb < 0 or non-perfect-square -> 0. */
static int	refSqrt(int nb)
{
    long	i;

    if (nb < 0)
        return (0);
    i = 0;
    while (i * i < (long)nb)
        i++;
    if (i * i == (long)nb)
        return ((int)i);
    return (0);
}

static void	assertSqrt(int nb)
{
    TEST_ASSERT_EQUAL_INT(refSqrt(nb), ft_sqrt(nb));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The two base cases. */
static void	testSqrtBaseCases(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(0));
    TEST_ASSERT_EQUAL_INT(1, ft_sqrt(1));
}

/* Documented perfect squares. */
static void	testSqrtKnownPerfectSquares(void)
{
    TEST_ASSERT_EQUAL_INT(2, ft_sqrt(4));
    TEST_ASSERT_EQUAL_INT(3, ft_sqrt(9));
    TEST_ASSERT_EQUAL_INT(4, ft_sqrt(16));
    TEST_ASSERT_EQUAL_INT(5, ft_sqrt(25));
    TEST_ASSERT_EQUAL_INT(7, ft_sqrt(49));
    TEST_ASSERT_EQUAL_INT(10, ft_sqrt(100));
    TEST_ASSERT_EQUAL_INT(12, ft_sqrt(144));
    TEST_ASSERT_EQUAL_INT(100, ft_sqrt(10000));
    TEST_ASSERT_EQUAL_INT(1000, ft_sqrt(1000000));
}

/* Non-perfect squares (fractional real root) return 0, including INT_MAX. */
static void	testSqrtNonPerfectSquares(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(2));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(3));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(5));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(8));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(15));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(99));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(101));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(INT_MAX));
}

/* Negative inputs have no real square root and return 0. */
static void	testSqrtNegativeReturnsZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(-1));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(-4));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(-100));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt(INT_MIN));
}

/* Top of the int range: 46340^2 is the largest representable perfect square;
** its neighbours and INT_MAX are not perfect squares. */
static void	testSqrtLargeValues(void)
{
    TEST_ASSERT_EQUAL_INT(30000, ft_sqrt(900000000));
    TEST_ASSERT_EQUAL_INT(46339, ft_sqrt((int)(46339L * 46339L)));
    TEST_ASSERT_EQUAL_INT(46340, ft_sqrt((int)(46340L * 46340L)));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt((int)(46340L * 46340L) - 1));
    TEST_ASSERT_EQUAL_INT(0, ft_sqrt((int)(46340L * 46340L) + 1));
}

/* Every value 0..10000 matches the reference (squares and non-squares mixed). */
static void	testSqrtContiguousSweep(void)
{
    int		nb;

    nb = 0;
    while (nb <= 10000)
    {
        assertSqrt(nb);
        nb++;
    }
}

/* Every perfect square r*r for r in 0..10000 returns exactly r. */
static void	testSqrtPerfectSquareSweep(void)
{
    long	r;

    r = 0;
    while (r <= 10000)
    {
        TEST_ASSERT_EQUAL_INT((int)r, ft_sqrt((int)(r * r)));
        r++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testSqrtBaseCases);
    RUN_TEST(testSqrtKnownPerfectSquares);
    RUN_TEST(testSqrtNonPerfectSquares);
    RUN_TEST(testSqrtNegativeReturnsZero);
    RUN_TEST(testSqrtLargeValues);
    RUN_TEST(testSqrtContiguousSweep);
    RUN_TEST(testSqrtPerfectSquareSweep);
}
