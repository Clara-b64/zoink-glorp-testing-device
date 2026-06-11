/*
** c07_ex04 -- ft_convert_base: char *ft_convert_base(char *nbr, char *base_from,
**                                                     char *base_to);
** Parses nbr (written in base_from) with the SAME rules as ft_atoi_base -- skip
** whitespace, consume a run of +/- signs, partial-parse the digits -- then
** returns a malloc'd string of that value written in base_to. A base is invalid
** if its length < 2, it has a duplicate symbol, or it contains '+', '-' or
** whitespace; if base_from or base_to is invalid the function returns NULL. The
** result carries at most one leading '-' (never '+' or whitespace). The value of
** nbr is assumed to stay within int. Inputs are read-only and must not be
** modified (most cases pass string literals, so a write would fault).
** The oracle reuses the c04_ex05 atoi_base parse plus a positional formatter.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

extern char	*ft_convert_base(char *nbr, char *base_from, char *base_to);

static int	refIsspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}

/* Valid base: length >= 2, no duplicate, no '+'/'-' and no whitespace symbol. */
static int	refIsValidBase(char *base)
{
    int	i;
    int	j;

    if ((int)strlen(base) < 2)
        return (0);
    i = 0;
    while (base[i] != '\0')
    {
        if (base[i] == '+' || base[i] == '-' || refIsspace(base[i]))
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

/* Index of c in base, or -1 if absent. */
static int	baseIndex(char c, char *base)
{
    int	i;

    i = 0;
    while (base[i] != '\0')
    {
        if (base[i] == c)
            return (i);
        i++;
    }
    return (-1);
}

/* Reference atoi_base: skip whitespace, multiple signs, partial parse. */
static int	refAtoiBase(char *str, char *base)
{
    int		i;
    int		sign;
    int		b;
    long	res;

    b = (int)strlen(base);
    i = 0;
    sign = 1;
    res = 0;
    while (refIsspace(str[i]))
        i++;
    while (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -sign;
        i++;
    }
    while (baseIndex(str[i], base) != -1)
    {
        res = res * b + baseIndex(str[i], base);
        i++;
    }
    return ((int)(res * sign));
}

/* Appends the base-b representation of v (>= 0) into out. */
static void	appendBase(char *out, int *len, long v, int b, char *base)
{
    if (v >= b)
        appendBase(out, len, v / b, b, base);
    out[(*len)++] = base[v % b];
}

/* Writes the signed base representation of value into out (one '-' if < 0). */
static void	genBaseRepr(char *out, int value, char *base)
{
    int		len;
    long	v;
    int		b;

    len = 0;
    v = value;
    b = (int)strlen(base);
    if (v < 0)
    {
        out[len++] = '-';
        v = -v;
    }
    appendBase(out, &len, v, b, base);
    out[len] = '\0';
}

/* Oracle: NULL if a base is invalid, else value(nbr in base_from) in base_to. */
static char	*refConvert(char *nbr, char *base_from, char *base_to)
{
    char	*out;

    if (!refIsValidBase(base_from) || !refIsValidBase(base_to))
        return (NULL);
    out = malloc(64);
    if (out == NULL)
        return (NULL);
    genBaseRepr(out, refAtoiBase(nbr, base_from), base_to);
    return (out);
}

/* Asserts ft_convert_base matches the oracle (NULL or exact string), frees both. */
static void	assertConvert(char *nbr, char *base_from, char *base_to)
{
    char	*expected;
    char	*result;

    expected = refConvert(nbr, base_from, base_to);
    result = ft_convert_base(nbr, base_from, base_to);
    if (expected == NULL)
    {
        TEST_ASSERT_NULL(result);
        return ;
    }
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);
    free(expected);
    free(result);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Converting within the same base reproduces the canonical spelling. */
static void	testConvertSameBase(void)
{
    assertConvert("42", "0123456789", "0123456789");
    assertConvert("ff", "0123456789abcdef", "0123456789abcdef");
    assertConvert("101010", "01", "01");
}

/* Documented cross-base conversions. */
static void	testConvertKnownValues(void)
{
    assertConvert("101010", "01", "0123456789");
    assertConvert("42", "0123456789", "01");
    assertConvert("ff", "0123456789abcdef", "0123456789");
    assertConvert("255", "0123456789", "0123456789abcdef");
    assertConvert("777", "01234567", "0123456789");
    assertConvert("511", "0123456789", "01234567");
}

/* A value of zero becomes the first symbol of base_to. */
static void	testConvertZero(void)
{
    assertConvert("0", "0123456789", "0123456789");
    assertConvert("0", "0123456789", "0123456789abcdef");
    assertConvert("0", "0123456789", "xyz");
    assertConvert("", "0123456789", "0123456789");
    assertConvert("xyz", "0123456789", "01");
}

/* Negative values carry exactly one '-' (no '+', no spaces). */
static void	testConvertNegative(void)
{
    assertConvert("-42", "0123456789", "0123456789");
    assertConvert("-42", "0123456789", "0123456789abcdef");
    assertConvert("-101010", "01", "0123456789");
    assertConvert("-ff", "0123456789abcdef", "01");
}

/* Whitespace then a run of signs, exactly like ft_atoi_base. */
static void	testConvertSignsAndWhitespace(void)
{
    assertConvert("  42", "0123456789", "0123456789");
    assertConvert("  -42", "0123456789", "0123456789");
    assertConvert("--42", "0123456789", "0123456789");
    assertConvert("---42", "0123456789", "0123456789");
    assertConvert("-+-2a", "0123456789abcdef", "0123456789");
    assertConvert("\t\n+42", "0123456789", "0123456789abcdef");
}

/* Parsing stops at the first character not in base_from. */
static void	testConvertPartialParse(void)
{
    assertConvert("42xyz", "0123456789", "0123456789");
    assertConvert("1g", "0123456789abcdef", "0123456789");
    assertConvert("101012", "01", "0123456789");
    assertConvert("ff!!", "0123456789abcdef", "01");
}

/* Bases other than the usual ones, including non-numeric symbols. */
static void	testConvertCustomBases(void)
{
    assertConvert("z", "xyz", "0123456789");
    assertConvert("ba", "ab", "0123456789");
    assertConvert("2", "0123456789", "ab");
    assertConvert("zzz", "xyz", "0123456789abcdef");
    assertConvert("-yx", "xyz", "0123456789");
}

/* The two int extremes, in several output bases. */
static void	testConvertIntBoundaries(void)
{
    assertConvert("2147483647", "0123456789", "0123456789");
    assertConvert("2147483647", "0123456789", "0123456789abcdef");
    assertConvert("2147483647", "0123456789", "01");
    assertConvert("-2147483648", "0123456789", "0123456789");
    assertConvert("-2147483648", "0123456789", "0123456789abcdef");
    assertConvert("-2147483648", "0123456789", "01");
}

/* An invalid base_from yields NULL. */
static void	testConvertInvalidBaseFrom(void)
{
    assertConvert("42", "", "0123456789");
    assertConvert("42", "0", "0123456789");
    assertConvert("42", "00", "0123456789");
    assertConvert("42", "01+", "0123456789");
    assertConvert("42", "01-", "0123456789");
    assertConvert("42", "0 1", "0123456789");
    assertConvert("42", "01\t", "0123456789");
}

/* An invalid base_to yields NULL. */
static void	testConvertInvalidBaseTo(void)
{
    assertConvert("42", "0123456789", "");
    assertConvert("42", "0123456789", "7");
    assertConvert("42", "0123456789", "aa");
    assertConvert("42", "0123456789", "01+");
    assertConvert("42", "0123456789", "0-1");
    assertConvert("42", "0123456789", "0 1");
}

/* Both bases invalid still yields NULL. */
static void	testConvertBothBasesInvalid(void)
{
    assertConvert("42", "", "");
    assertConvert("42", "+", "-");
    assertConvert("42", "z", "9");
}

/* The inputs are read-only and must come back unchanged. */
static void	testConvertDoesNotModifyInputs(void)
{
    char	nbr[] = "  -2a";
    char	from[] = "0123456789abcdef";
    char	to[] = "0123456789";
    char	*result;

    result = ft_convert_base(nbr, from, to);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("-42", result);
    TEST_ASSERT_EQUAL_STRING("  -2a", nbr);
    TEST_ASSERT_EQUAL_STRING("0123456789abcdef", from);
    TEST_ASSERT_EQUAL_STRING("0123456789", to);
    free(result);
}

/* Each call returns its own free-able allocation. */
static void	testConvertReturnsFreshAllocation(void)
{
    char	*a;
    char	*b;

    a = ft_convert_base("42", "0123456789", "01");
    b = ft_convert_base("42", "0123456789", "01");
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_TRUE(a != b);
    TEST_ASSERT_EQUAL_STRING("101010", a);
    TEST_ASSERT_EQUAL_STRING("101010", b);
    free(a);
    free(b);
}

/* Every value -200..200 (as decimal) converts to five bases matching the oracle. */
static void	testConvertSweep(void)
{
    char	nbr[16];
    char	*bases[5];
    int		v;
    int		bi;

    bases[0] = "01";
    bases[1] = "01234567";
    bases[2] = "0123456789";
    bases[3] = "0123456789abcdef";
    bases[4] = "xyz";
    v = -200;
    while (v <= 200)
    {
        snprintf(nbr, sizeof(nbr), "%d", v);
        bi = 0;
        while (bi < 5)
        {
            assertConvert(nbr, "0123456789", bases[bi]);
            bi++;
        }
        v++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testConvertSameBase);
    RUN_TEST(testConvertKnownValues);
    RUN_TEST(testConvertZero);
    RUN_TEST(testConvertNegative);
    RUN_TEST(testConvertSignsAndWhitespace);
    RUN_TEST(testConvertPartialParse);
    RUN_TEST(testConvertCustomBases);
    RUN_TEST(testConvertIntBoundaries);
    RUN_TEST(testConvertInvalidBaseFrom);
    RUN_TEST(testConvertInvalidBaseTo);
    RUN_TEST(testConvertBothBasesInvalid);
    RUN_TEST(testConvertDoesNotModifyInputs);
    RUN_TEST(testConvertReturnsFreshAllocation);
    RUN_TEST(testConvertSweep);
}
