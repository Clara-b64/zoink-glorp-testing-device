/*
** c01_ex04 -- ft_ultimate_div_mod: void ft_ultimate_div_mod(int *a, int *b);
** Divides *a by *b in place: *a receives the quotient, *b the remainder. Both
** inputs must be read before either is overwritten. Division by zero is
** undefined and not exercised; the C operators / and % are the oracle.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_ultimate_div_mod(int *a, int *b);

/* Seeds a and b, runs the function in place, and checks both results against
** the C operators applied to the ORIGINAL inputs (so an impl that overwrites
** *a before computing the remainder is caught). */
static void	assertUltimateDivMod(int aval, int bval)
{
    int	a;
    int	b;

    a = aval;
    b = bval;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(aval / bval, a);
    TEST_ASSERT_EQUAL_INT(aval % bval, b);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain positive division: quotient in *a, remainder in *b. Also the key
** ordering case -- *a is overwritten with 8, but *b must still become 2. */
static void	testUltimateDivModBasic(void)
{
    int	a;
    int	b;

    a = 42;
    b = 5;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(8, a);
    TEST_ASSERT_EQUAL_INT(2, b);
}

/* An exact division leaves a zero remainder. */
static void	testUltimateDivModExact(void)
{
    int	a;
    int	b;

    a = 20;
    b = 4;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(5, a);
    TEST_ASSERT_EQUAL_INT(0, b);
}

/* When |a| < |b| the quotient is 0 and the remainder is the original a. */
static void	testUltimateDivModDividendSmaller(void)
{
    int	a;
    int	b;

    a = 3;
    b = 7;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(0, a);
    TEST_ASSERT_EQUAL_INT(3, b);
}

/* Zero dividend gives zero quotient and remainder. */
static void	testUltimateDivModZeroDividend(void)
{
    int	a;
    int	b;

    a = 0;
    b = 7;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(0, a);
    TEST_ASSERT_EQUAL_INT(0, b);
}

/* Negative dividend: quotient truncates toward zero, remainder negative. */
static void	testUltimateDivModNegativeDividend(void)
{
    int	a;
    int	b;

    a = -42;
    b = 5;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(-8, a);
    TEST_ASSERT_EQUAL_INT(-2, b);
}

/* Negative divisor: quotient negative, remainder takes the dividend's sign. */
static void	testUltimateDivModNegativeDivisor(void)
{
    int	a;
    int	b;

    a = 42;
    b = -5;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(-8, a);
    TEST_ASSERT_EQUAL_INT(2, b);
}

/* Both negative: quotient positive, remainder negative. */
static void	testUltimateDivModBothNegative(void)
{
    int	a;
    int	b;

    a = -42;
    b = -5;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(8, a);
    TEST_ASSERT_EQUAL_INT(-2, b);
}

/* Equal values: quotient 1, remainder 0. */
static void	testUltimateDivModEqualValues(void)
{
    int	a;
    int	b;

    a = 5;
    b = 5;
    ft_ultimate_div_mod(&a, &b);
    TEST_ASSERT_EQUAL_INT(1, a);
    TEST_ASSERT_EQUAL_INT(0, b);
}

/* Only *a and *b change; surrounding memory is untouched. */
static void	testUltimateDivModDoesNotTouchNeighbours(void)
{
    int	buf[4];

    buf[0] = 11;
    buf[1] = 42;
    buf[2] = 5;
    buf[3] = 22;
    ft_ultimate_div_mod(&buf[1], &buf[2]);
    TEST_ASSERT_EQUAL_INT(11, buf[0]);
    TEST_ASSERT_EQUAL_INT(8, buf[1]);
    TEST_ASSERT_EQUAL_INT(2, buf[2]);
    TEST_ASSERT_EQUAL_INT(22, buf[3]);
}

/* Every sign combination over a dense range matches the C operators. */
static void	testUltimateDivModMatchesCOperatorsOverRange(void)
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
                assertUltimateDivMod(a, b);
            b++;
        }
        a++;
    }
}

/* Int boundaries that do not overflow (INT_MIN / -1 is excluded as UB). */
static void	testUltimateDivModAtBoundaries(void)
{
    assertUltimateDivMod(INT_MAX, 1);
    assertUltimateDivMod(INT_MIN, 1);
    assertUltimateDivMod(INT_MAX, -1);
    assertUltimateDivMod(INT_MAX, 2);
    assertUltimateDivMod(INT_MIN, 2);
    assertUltimateDivMod(INT_MIN, 7);
    assertUltimateDivMod(INT_MAX, INT_MAX);
    assertUltimateDivMod(INT_MIN, INT_MIN);
}

void	glorpRun(void)
{
    RUN_TEST(testUltimateDivModBasic);
    RUN_TEST(testUltimateDivModExact);
    RUN_TEST(testUltimateDivModDividendSmaller);
    RUN_TEST(testUltimateDivModZeroDividend);
    RUN_TEST(testUltimateDivModNegativeDividend);
    RUN_TEST(testUltimateDivModNegativeDivisor);
    RUN_TEST(testUltimateDivModBothNegative);
    RUN_TEST(testUltimateDivModEqualValues);
    RUN_TEST(testUltimateDivModDoesNotTouchNeighbours);
    RUN_TEST(testUltimateDivModMatchesCOperatorsOverRange);
    RUN_TEST(testUltimateDivModAtBoundaries);
}
