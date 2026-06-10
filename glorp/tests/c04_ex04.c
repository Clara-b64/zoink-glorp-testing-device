/*
** c04_ex04 -- ft_putnbr_base: void ft_putnbr_base(int nbr, char *base);
** Writes nbr to stdout (fd 1) in the base described by `base`, whose characters
** are the digit symbols (length = the base). Symbols may be anything printable;
** negatives are written with a leading '-' (which is why '-' cannot be a base
** symbol). If `base` is invalid the function writes NOTHING.
**
** Invalid base (per the exercise spec): length < 2, a duplicate symbol, or a
** symbol that is '+' or '-'. (The reference additionally rejects non-printable
** symbols; that is allowed by the spec's "not limited to" but is not relied on
** here -- no such bases are tested.)
**
** The oracle refExpected builds the expected output (empty for an invalid base,
** for ANY nbr including 0). Output is captured from fd 1 and compared.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>
#include <limits.h>

extern void	ft_putnbr_base(int nbr, char *base);

/* Valid per the spec: length >= 2, no duplicate symbol, no '+' or '-'. */
static int	refIsValidBase(char *base)
{
    int	i;
    int	j;

    if ((int)strlen(base) < 2)
        return (0);
    i = 0;
    while (base[i] != '\0')
    {
        if (base[i] == '+' || base[i] == '-')
            return (0);
        j = i + 1;
        while (base[j] != '\0')
        {
            if (base[i] == base[j])
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

/* Appends the base-b representation of nb (>= 0) into out. */
static void	refBuild(char *out, int *len, long nb, int b, char *base)
{
    if (nb >= b)
        refBuild(out, len, nb / b, b, base);
    out[(*len)++] = base[nb % b];
}

/* Builds the expected output for ft_putnbr_base(nbr, base); returns its length
** (0 for an invalid base, regardless of nbr). */
static int	refExpected(char *out, int nbr, char *base)
{
    int		len;
    int		b;
    long	nb;

    len = 0;
    if (!refIsValidBase(base))
    {
        out[0] = '\0';
        return (0);
    }
    b = (int)strlen(base);
    nb = (long)nbr;
    if (nb < 0)
    {
        out[len++] = '-';
        nb = -nb;
    }
    refBuild(out, &len, nb, b, base);
    out[len] = '\0';
    return (len);
}

/* Runs ft_putnbr_base with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePutnbrBase(int nbr, char *base, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

    saved = dup(STDOUT_FILENO);
    if (saved == -1)
        return (-1);
    if (pipe(pfd) == -1)
    {
        close(saved);
        return (-1);
    }
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    ft_putnbr_base(nbr, base);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

/* Asserts ft_putnbr_base output matches the oracle for nbr/base. */
static void	assertPutnbrBase(int nbr, char *base)
{
    char	got[128];
    char	exp[128];
    int		gl;
    int		el;

    gl = capturePutnbrBase(nbr, base, got, sizeof(got));
    el = refExpected(exp, nbr, base);
    TEST_ASSERT_EQUAL_INT(el, gl);
    if (el > 0)
        TEST_ASSERT_EQUAL_MEMORY(exp, got, el);
}

/* Asserts ft_putnbr_base prints exactly `expected`. */
static void	expectOutput(int nbr, char *base, char *expected)
{
    char	got[128];
    int		gl;

    gl = capturePutnbrBase(nbr, base, got, sizeof(got));
    TEST_ASSERT_EQUAL_INT((int)strlen(expected), gl);
    if (expected[0] != '\0')
        TEST_ASSERT_EQUAL_MEMORY(expected, got, (int)strlen(expected));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Decimal, with documented expected strings. */
static void	testPutnbrBaseDecimalKnown(void)
{
    expectOutput(42, "0123456789", "42");
    expectOutput(-42, "0123456789", "-42");
    expectOutput(1000000, "0123456789", "1000000");
    expectOutput(0, "0123456789", "0");
}

/* Hexadecimal. */
static void	testPutnbrBaseHexKnown(void)
{
    expectOutput(42, "0123456789abcdef", "2a");
    expectOutput(255, "0123456789abcdef", "ff");
    expectOutput(-255, "0123456789abcdef", "-ff");
    expectOutput(0, "0123456789abcdef", "0");
}

/* Binary. */
static void	testPutnbrBaseBinaryKnown(void)
{
    expectOutput(5, "01", "101");
    expectOutput(-5, "01", "-101");
    expectOutput(8, "01", "1000");
    expectOutput(0, "01", "0");
}

/* An arbitrary symbol set behaves like any other base. */
static void	testPutnbrBaseArbitraryKnown(void)
{
    expectOutput(1, "ab", "b");
    expectOutput(2, "ab", "ba");
    expectOutput(-2, "ab", "-ba");
    expectOutput(0, "ab", "a");
}

/* Zero prints the first symbol of any VALID base. */
static void	testPutnbrBaseZeroValidBases(void)
{
    assertPutnbrBase(0, "0123456789");
    assertPutnbrBase(0, "01");
    assertPutnbrBase(0, "0123456789abcdef");
    assertPutnbrBase(0, "xyz");
}

/* The int boundaries in several bases (oracle uses long, so no overflow). */
static void	testPutnbrBaseIntBoundaries(void)
{
    expectOutput(INT_MAX, "0123456789", "2147483647");
    expectOutput(INT_MIN, "0123456789", "-2147483648");
    expectOutput(INT_MAX, "0123456789abcdef", "7fffffff");
    expectOutput(INT_MIN, "0123456789abcdef", "-80000000");
    assertPutnbrBase(INT_MAX, "01");
    assertPutnbrBase(INT_MIN, "01");
    assertPutnbrBase(INT_MIN, "0123456789abcdefghijklmnopqrstuvwxyz");
}

/* Dense sweep in decimal. */
static void	testPutnbrBaseDenseDecimal(void)
{
    int	n;

    n = -1000;
    while (n <= 1000)
    {
        assertPutnbrBase(n, "0123456789");
        n++;
    }
}

/* Dense sweep in binary and hex. */
static void	testPutnbrBaseDenseBinaryHex(void)
{
    int	n;

    n = -500;
    while (n <= 500)
    {
        assertPutnbrBase(n, "01");
        assertPutnbrBase(n, "0123456789abcdef");
        n++;
    }
}

/* A large arbitrary base (base 36). */
static void	testPutnbrBaseLargeBase(void)
{
    char	*base36;
    int		n;

    base36 = "0123456789abcdefghijklmnopqrstuvwxyz";
    expectOutput(35, base36, "z");
    expectOutput(36, base36, "10");
    expectOutput(1295, base36, "zz");
    n = -200;
    while (n <= 200)
    {
        assertPutnbrBase(n, base36);
        n++;
    }
}

/* Invalid base (length < 2): writes nothing (nbr != 0). */
static void	testPutnbrBaseInvalidLength(void)
{
    assertPutnbrBase(42, "");
    assertPutnbrBase(42, "0");
    assertPutnbrBase(-7, "x");
}

/* Invalid base (duplicate symbol): writes nothing (nbr != 0). */
static void	testPutnbrBaseInvalidDuplicate(void)
{
    assertPutnbrBase(42, "00");
    assertPutnbrBase(42, "0123456780");
    assertPutnbrBase(42, "aba");
    assertPutnbrBase(42, "ilove42somuch");
}

/* Invalid base (contains '+' or '-'): writes nothing (nbr != 0). */
static void	testPutnbrBaseInvalidSignChars(void)
{
    assertPutnbrBase(42, "+0123456789");
    assertPutnbrBase(42, "0123456789-");
    assertPutnbrBase(42, "012-3");
    assertPutnbrBase(42, "+-0123456789abcdef");
}

/* Per the spec, an invalid base prints nothing even when nbr == 0.
** NOTE: the picine reference checks nbr == 0 BEFORE validating the base, so it
** prints base[0] here instead -- this test will FAIL the reference, flagging
** that real deviation from the spec. */
static void	testPutnbrBaseZeroWithInvalidBasePrintsNothing(void)
{
    assertPutnbrBase(0, "");
    assertPutnbrBase(0, "0");
    assertPutnbrBase(0, "00");
    assertPutnbrBase(0, "+-0123456789abcdef");
}

void	glorpRun(void)
{
    RUN_TEST(testPutnbrBaseDecimalKnown);
    RUN_TEST(testPutnbrBaseHexKnown);
    RUN_TEST(testPutnbrBaseBinaryKnown);
    RUN_TEST(testPutnbrBaseArbitraryKnown);
    RUN_TEST(testPutnbrBaseZeroValidBases);
    RUN_TEST(testPutnbrBaseIntBoundaries);
    RUN_TEST(testPutnbrBaseDenseDecimal);
    RUN_TEST(testPutnbrBaseDenseBinaryHex);
    RUN_TEST(testPutnbrBaseLargeBase);
    RUN_TEST(testPutnbrBaseInvalidLength);
    RUN_TEST(testPutnbrBaseInvalidDuplicate);
    RUN_TEST(testPutnbrBaseInvalidSignChars);
    RUN_TEST(testPutnbrBaseZeroWithInvalidBasePrintsNothing);
}
