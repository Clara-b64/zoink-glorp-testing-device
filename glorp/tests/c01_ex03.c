/*
** c01_ex03 -- ft_div_mod: void ft_div_mod(int a, int b, int *div, int *mod);
** Stores a / b into *div and a % b into *mod. Division by zero is undefined and
** not exercised. C integer division truncates toward zero, so the C operators
** / and % are used as the oracle.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_div_mod(int a, int b, int *div, int *mod);

/* Calls ft_div_mod into sentinel-seeded outputs and checks both against the C
** operators (so both writes are verified, not just one). */
static void	assertDivMod(int a, int b)
{
    int	d;
    int	m;

    d = 123456;
    m = 654321;
    ft_div_mod(a, b, &d, &m);
    TEST_ASSERT_EQUAL_INT(a / b, d);
    TEST_ASSERT_EQUAL_INT(a % b, m);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain positive division with a remainder. */
static void	testDivModBasic(void)
{
    int	d;
    int	m;

    ft_div_mod(42, 5, &d, &m);
    TEST_ASSERT_EQUAL_INT(8, d);
    TEST_ASSERT_EQUAL_INT(2, m);
}

/* An exact division leaves a zero remainder. */
static void	testDivModExact(void)
{
    int	d;
    int	m;

    ft_div_mod(20, 4, &d, &m);
    TEST_ASSERT_EQUAL_INT(5, d);
    TEST_ASSERT_EQUAL_INT(0, m);
}

/* When |a| < |b| the quotient is 0 and the remainder is a. */
static void	testDivModDividendSmallerThanDivisor(void)
{
    int	d;
    int	m;

    ft_div_mod(3, 7, &d, &m);
    TEST_ASSERT_EQUAL_INT(0, d);
    TEST_ASSERT_EQUAL_INT(3, m);
}

/* Zero dividend gives zero quotient and remainder. */
static void	testDivModZeroDividend(void)
{
    int	d;
    int	m;

    ft_div_mod(0, 7, &d, &m);
    TEST_ASSERT_EQUAL_INT(0, d);
    TEST_ASSERT_EQUAL_INT(0, m);
}

/* Negative dividend: quotient truncates toward zero, remainder is negative. */
static void	testDivModNegativeDividend(void)
{
    int	d;
    int	m;

    ft_div_mod(-42, 5, &d, &m);
    TEST_ASSERT_EQUAL_INT(-8, d);
    TEST_ASSERT_EQUAL_INT(-2, m);
}

/* Negative divisor: quotient is negative, remainder takes the dividend's sign. */
static void	testDivModNegativeDivisor(void)
{
    int	d;
    int	m;

    ft_div_mod(42, -5, &d, &m);
    TEST_ASSERT_EQUAL_INT(-8, d);
    TEST_ASSERT_EQUAL_INT(2, m);
}

/* Both negative: quotient is positive, remainder is negative. */
static void	testDivModBothNegative(void)
{
    int	d;
    int	m;

    ft_div_mod(-42, -5, &d, &m);
    TEST_ASSERT_EQUAL_INT(8, d);
    TEST_ASSERT_EQUAL_INT(-2, m);
}

/* Both outputs are written even when the quotient is zero (catches a missing
** *div or *mod assignment). */
static void	testDivModWritesBothOutputs(void)
{
    int	d;
    int	m;

    d = -999;
    m = -999;
    ft_div_mod(3, 7, &d, &m);
    TEST_ASSERT_EQUAL_INT(0, d);
    TEST_ASSERT_EQUAL_INT(3, m);
}

/* Only *div and *mod change; surrounding memory is untouched. */
static void	testDivModDoesNotTouchNeighbours(void)
{
    int	buf[4];

    buf[0] = 11;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 22;
    ft_div_mod(42, 5, &buf[1], &buf[2]);
    TEST_ASSERT_EQUAL_INT(11, buf[0]);
    TEST_ASSERT_EQUAL_INT(8, buf[1]);
    TEST_ASSERT_EQUAL_INT(2, buf[2]);
    TEST_ASSERT_EQUAL_INT(22, buf[3]);
}

/* Every sign combination over a dense range matches the C operators. */
static void	testDivModMatchesCOperatorsOverRange(void)
{
    int	a;
    int	b;

    a = -60;
    while (a <= 60)
    {
        b = -12;
        while (b <= 12)
        {
            if (b != 0)
                assertDivMod(a, b);
            b++;
        }
        a++;
    }
}

/* Int boundaries that do not overflow (INT_MIN / -1 is excluded as UB). */
static void	testDivModAtBoundaries(void)
{
    assertDivMod(INT_MAX, 1);
    assertDivMod(INT_MIN, 1);
    assertDivMod(INT_MAX, -1);
    assertDivMod(INT_MAX, 2);
    assertDivMod(INT_MIN, 2);
    assertDivMod(INT_MIN, 7);
    assertDivMod(INT_MAX, INT_MAX);
    assertDivMod(INT_MIN, INT_MIN);
}

void	glorpRun(void)
{
    RUN_TEST(testDivModBasic);
    RUN_TEST(testDivModExact);
    RUN_TEST(testDivModDividendSmallerThanDivisor);
    RUN_TEST(testDivModZeroDividend);
    RUN_TEST(testDivModNegativeDividend);
    RUN_TEST(testDivModNegativeDivisor);
    RUN_TEST(testDivModBothNegative);
    RUN_TEST(testDivModWritesBothOutputs);
    RUN_TEST(testDivModDoesNotTouchNeighbours);
    RUN_TEST(testDivModMatchesCOperatorsOverRange);
    RUN_TEST(testDivModAtBoundaries);
}
