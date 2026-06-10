/*
** c05_ex02 -- ft_iterative_power: int ft_iterative_power(int nb, int power);
** Returns nb raised to `power`, iteratively. power < 0 -> 0. power == 0 -> 1 for
** any nb (including 0^0 = 1, by convention). Overflow is undefined, so the
** assert helper computes the expected value in long and skips any case that
** leaves the int range.
*/

#include "runner.h"
#include <limits.h>

extern int	ft_iterative_power(int nb, int power);

/* Asserts ft_iterative_power(nb, power) for well-defined cases; power < 0 must
** be 0, and combinations that overflow int are skipped (undefined). */
static void	assertPower(int nb, int power)
{
    long	r;
    int		i;

    if (power < 0)
    {
        TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(nb, power));
        return ;
    }
    r = 1;
    i = 0;
    while (i < power)
    {
        r *= nb;
        if (r > INT_MAX || r < INT_MIN)
            return ;
        i++;
    }
    TEST_ASSERT_EQUAL_INT((int)r, ft_iterative_power(nb, power));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Exponent 0 is 1 for any base, including 0^0. */
static void	testPowerZeroExponent(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(0, 0));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(1, 0));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(5, 0));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(-3, 0));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(INT_MAX, 0));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(INT_MIN, 0));
}

/* Documented known values. */
static void	testPowerKnownValues(void)
{
    TEST_ASSERT_EQUAL_INT(1024, ft_iterative_power(2, 10));
    TEST_ASSERT_EQUAL_INT(1073741824, ft_iterative_power(2, 30));
    TEST_ASSERT_EQUAL_INT(243, ft_iterative_power(3, 5));
    TEST_ASSERT_EQUAL_INT(125, ft_iterative_power(5, 3));
    TEST_ASSERT_EQUAL_INT(1000000000, ft_iterative_power(10, 9));
    TEST_ASSERT_EQUAL_INT(49, ft_iterative_power(7, 2));
}

/* 0 to a positive power is 0. */
static void	testPowerBaseZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(0, 1));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(0, 5));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(0, 1000));
}

/* 1 to any power is 1. */
static void	testPowerBaseOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(1, 1));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(1, 1000000));
}

/* -1 alternates: even power -> 1, odd power -> -1. */
static void	testPowerBaseNegativeOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(-1, 0));
    TEST_ASSERT_EQUAL_INT(-1, ft_iterative_power(-1, 1));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(-1, 2));
    TEST_ASSERT_EQUAL_INT(-1, ft_iterative_power(-1, 99));
    TEST_ASSERT_EQUAL_INT(1, ft_iterative_power(-1, 100));
}

/* Negative bases: sign follows the parity of the exponent. */
static void	testPowerNegativeBase(void)
{
    TEST_ASSERT_EQUAL_INT(4, ft_iterative_power(-2, 2));
    TEST_ASSERT_EQUAL_INT(-8, ft_iterative_power(-2, 3));
    TEST_ASSERT_EQUAL_INT(1024, ft_iterative_power(-2, 10));
    TEST_ASSERT_EQUAL_INT(-27, ft_iterative_power(-3, 3));
    TEST_ASSERT_EQUAL_INT(25, ft_iterative_power(-5, 2));
}

/* A negative exponent always returns 0. */
static void	testPowerNegativeExponent(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(5, -1));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(2, -10));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(0, -1));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(-3, -2));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(INT_MAX, -1));
    TEST_ASSERT_EQUAL_INT(0, ft_iterative_power(7, INT_MIN));
}

/* Sweep over bases -10..10 and exponents 0..30 (overflowing cases skipped). */
static void	testPowerSweep(void)
{
    int	nb;
    int	power;

    nb = -10;
    while (nb <= 10)
    {
        power = 0;
        while (power <= 30)
        {
            assertPower(nb, power);
            power++;
        }
        nb++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPowerZeroExponent);
    RUN_TEST(testPowerKnownValues);
    RUN_TEST(testPowerBaseZero);
    RUN_TEST(testPowerBaseOne);
    RUN_TEST(testPowerBaseNegativeOne);
    RUN_TEST(testPowerNegativeBase);
    RUN_TEST(testPowerNegativeExponent);
    RUN_TEST(testPowerSweep);
}
