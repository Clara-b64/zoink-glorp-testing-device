/*
** c08_ex02 -- ft_abs.h: a HEADER defining ABS(Value), a macro that evaluates to
** the absolute value of its argument at runtime (the "replace with" wording just
** describes macro substitution). ABS(v) == (v < 0 ? -v : v).
**
** ABS(INT_MIN) is NOT tested: -INT_MIN overflows int (undefined), so the largest
** magnitude exercised is INT_MIN + 1 (whose absolute value is INT_MAX). The
** reference macros are unparenthesized, so they are correct for plain values and
** variables but not for expression arguments like ABS(a - b); the subject only
** requires plain values, so only those are tested.
**
** Built via the header-test path (compiled with -I<piscine ex02 dir>); it never
** compiles the provided ft_abs.c.
*/

#include "runner.h"
#include "ft_abs.h"
#include <limits.h>

static int	refAbs(int v)
{
    return (v < 0 ? -v : v);
}

/* ABS(v) for a (side-effect-free) variable equals the reference. */
static void	assertAbs(int v)
{
    TEST_ASSERT_EQUAL_INT(refAbs(v), ABS(v));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Direct literal arguments, positive and negative (negative literals expand to
** two separate '-' tokens, not a decrement). */
static void	testAbsLiterals(void)
{
    TEST_ASSERT_EQUAL_INT(5, ABS(5));
    TEST_ASSERT_EQUAL_INT(5, ABS(-5));
    TEST_ASSERT_EQUAL_INT(0, ABS(0));
    TEST_ASSERT_EQUAL_INT(1, ABS(-1));
    TEST_ASSERT_EQUAL_INT(100, ABS(-100));
    TEST_ASSERT_EQUAL_INT(42, ABS(42));
}

/* ABS applied to variables holding positive, negative and zero. */
static void	testAbsVariables(void)
{
    int	pos;
    int	neg;
    int	zero;

    pos = 7;
    neg = -7;
    zero = 0;
    TEST_ASSERT_EQUAL_INT(7, ABS(pos));
    TEST_ASSERT_EQUAL_INT(7, ABS(neg));
    TEST_ASSERT_EQUAL_INT(0, ABS(zero));
}

/* Large magnitudes, up to the int extremes (INT_MIN excluded). */
static void	testAbsBoundaries(void)
{
    assertAbs(INT_MAX);
    assertAbs(INT_MIN + 1);
    assertAbs(2000000000);
    assertAbs(-2000000000);
}

/* Every value in a dense range matches the reference. */
static void	testAbsSweep(void)
{
    int	v;

    v = -1000;
    while (v <= 1000)
    {
        assertAbs(v);
        v++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testAbsLiterals);
    RUN_TEST(testAbsVariables);
    RUN_TEST(testAbsBoundaries);
    RUN_TEST(testAbsSweep);
}
