/*
** c04_ex05 -- ft_atoi_base: int ft_atoi_base(char *str, char *base);
** Parses the start of str, written in the base described by `base`, the same
** way ft_atoi parses base 10:
**   1. skip whitespace: ' ' '\t' '\n' '\v' '\f' '\r'
**   2. consume a run of '+'/'-' signs; each '-' flips the sign
**   3. read base "digits" (chars present in base) and STOP at the first char
**      that is not a base digit (atoi-like partial parse)
**   4. return value * sign; no leading digits -> 0
** If the base is invalid the function returns 0. Invalid base (per the spec):
** length < 2, a duplicate symbol, or a symbol that is '+', '-' or whitespace.
**
** The oracle refAtoiBase replicates this with a long accumulator (UB-free for
** in-range results). Inputs stay within the int range (overflow is undefined).
*/

#include "runner.h"
#include <string.h>
#include <limits.h>

extern int	ft_atoi_base(char *str, char *base);

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

/* Reference atoi_base (partial parse, multiple signs). */
static int	refAtoiBase(char *str, char *base)
{
    int		i;
    int		sign;
    int		b;
    long	res;

    if (!refIsValidBase(base))
        return (0);
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

/* Asserts ft_atoi_base matches the reference parse. */
static void	assertAtoiBase(char *str, char *base)
{
    TEST_ASSERT_EQUAL_INT(refAtoiBase(str, base), ft_atoi_base(str, base));
}

/* Appends the base-b representation of v (>= 0) into out. */
static void	appendBase(char *out, int *len, long v, int b, char *base)
{
    if (v >= b)
        appendBase(out, len, v / b, b, base);
    out[(*len)++] = base[v % b];
}

/* Writes the signed base representation of `value` into out. */
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

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Hexadecimal, clean strings (all valid base digits). */
static void	testAtoiBaseHexClean(void)
{
    assertAtoiBase("2a", "0123456789abcdef");
    assertAtoiBase("ff", "0123456789abcdef");
    assertAtoiBase("-2a", "0123456789abcdef");
    assertAtoiBase("0", "0123456789abcdef");
    assertAtoiBase("10", "0123456789abcdef");
}

/* Decimal base behaves like base-10 atoi for clean numeric strings. */
static void	testAtoiBaseDecimalClean(void)
{
    assertAtoiBase("42", "0123456789");
    assertAtoiBase("-42", "0123456789");
    assertAtoiBase("0", "0123456789");
    assertAtoiBase("000123", "0123456789");
}

/* Binary. */
static void	testAtoiBaseBinary(void)
{
    assertAtoiBase("101", "01");
    assertAtoiBase("-1000", "01");
    assertAtoiBase("0", "01");
}

/* An arbitrary symbol set works like any other base. */
static void	testAtoiBaseArbitraryBase(void)
{
    assertAtoiBase("ba", "ab");
    assertAtoiBase("-ba", "ab");
    assertAtoiBase("cat", "ilovecats");
    assertAtoiBase("love", "ilovecats");
}

/* Leading whitespace is skipped (each whitespace char and mixes). */
static void	testAtoiBaseLeadingWhitespace(void)
{
    assertAtoiBase("   2a", "0123456789abcdef");
    assertAtoiBase("\t\n\v\f\r 2a", "0123456789abcdef");
    assertAtoiBase(" \t -2a", "0123456789abcdef");
}

/* Multiple signs are counted, like ft_atoi (the reference's own examples). */
static void	testAtoiBaseMultipleSigns(void)
{
    assertAtoiBase("--2a", "0123456789abcdef");
    assertAtoiBase("---2a", "0123456789abcdef");
    assertAtoiBase("+-2a", "0123456789abcdef");
    assertAtoiBase("-+-2a", "0123456789abcdef");
    assertAtoiBase("   --------+-2a", "0123456789abcdef");
}

/* No leading base digits yields 0. */
static void	testAtoiBaseNoLeadingDigits(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_atoi_base("", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi_base("ZZ", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi_base("   ", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi_base("+-", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi_base("g", "0123456789abcdef"));
}

/* The int boundaries (well-defined). */
static void	testAtoiBaseBoundaries(void)
{
    TEST_ASSERT_EQUAL_INT(INT_MAX, ft_atoi_base("7fffffff", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(INT_MIN, ft_atoi_base("-80000000", "0123456789abcdef"));
    TEST_ASSERT_EQUAL_INT(2147483647, ft_atoi_base("2147483647", "0123456789"));
    TEST_ASSERT_EQUAL_INT(INT_MIN, ft_atoi_base("-2147483648", "0123456789"));
    assertAtoiBase("7fffffff", "0123456789abcdef");
    assertAtoiBase("-80000000", "0123456789abcdef");
}

/* An invalid base always yields 0 (any str). */
static void	testAtoiBaseInvalidBase(void)
{
    assertAtoiBase("2a", "");
    assertAtoiBase("2a", "0");
    assertAtoiBase("123", "00");
    assertAtoiBase("123", "0120");
    assertAtoiBase("123", "+0123456789");
    assertAtoiBase("123", "0123456789-");
    assertAtoiBase("123", "01 23");
    assertAtoiBase("123", "01\t23");
}

/* PARTIAL: parsing stops at the first non-base char and returns what it read.
** NOTE: a STRICT implementation (like picine_1) returns 0 here instead, so
** these assertions will fail such a reference -- the chosen contract. */
static void	testAtoiBaseStopsAtNonBaseChar(void)
{
    assertAtoiBase("2aZZ", "0123456789abcdef");
    assertAtoiBase("ffG", "0123456789abcdef");
    assertAtoiBase("2a ", "0123456789abcdef");
    assertAtoiBase("  -2a hello", "0123456789abcdef");
    assertAtoiBase("101 01", "01");
    assertAtoiBase("420z", "0123456789");
}

/* Dense round-trip: the base representation of n parses back to n. */
static void	testAtoiBaseDenseRoundTrip(void)
{
    char	buf[64];
    int		n;

    n = -1000;
    while (n <= 1000)
    {
        genBaseRepr(buf, n, "01");
        assertAtoiBase(buf, "01");
        genBaseRepr(buf, n, "0123456789");
        assertAtoiBase(buf, "0123456789");
        genBaseRepr(buf, n, "0123456789abcdef");
        assertAtoiBase(buf, "0123456789abcdef");
        n++;
    }
}

/* Clean combinatorial sweep: whitespace x signs x magnitude x base. */
static void	testAtoiBaseCleanSweep(void)
{
    char	*ws[4];
    char	*signs[9];
    char	*bases[3];
    int		mags[6];
    char	body[48];
    char	buf[80];
    int		a;
    int		s;
    int		m;
    int		bi;

    ws[0] = "";
    ws[1] = " ";
    ws[2] = "\t";
    ws[3] = "  \t\n\v\f\r ";
    signs[0] = "";
    signs[1] = "+";
    signs[2] = "-";
    signs[3] = "++";
    signs[4] = "--";
    signs[5] = "+-";
    signs[6] = "-+";
    signs[7] = "---";
    signs[8] = "+-+-+";
    bases[0] = "01";
    bases[1] = "0123456789";
    bases[2] = "0123456789abcdef";
    mags[0] = 0;
    mags[1] = 1;
    mags[2] = 42;
    mags[3] = 255;
    mags[4] = 123456;
    mags[5] = 1000000;
    bi = 0;
    while (bi < 3)
    {
        m = 0;
        while (m < 6)
        {
            genBaseRepr(body, mags[m], bases[bi]);
            a = 0;
            while (a < 4)
            {
                s = 0;
                while (s < 9)
                {
                    buf[0] = '\0';
                    strcat(buf, ws[a]);
                    strcat(buf, signs[s]);
                    strcat(buf, body);
                    assertAtoiBase(buf, bases[bi]);
                    s++;
                }
                a++;
            }
            m++;
        }
        bi++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testAtoiBaseHexClean);
    RUN_TEST(testAtoiBaseDecimalClean);
    RUN_TEST(testAtoiBaseBinary);
    RUN_TEST(testAtoiBaseArbitraryBase);
    RUN_TEST(testAtoiBaseLeadingWhitespace);
    RUN_TEST(testAtoiBaseMultipleSigns);
    RUN_TEST(testAtoiBaseNoLeadingDigits);
    RUN_TEST(testAtoiBaseBoundaries);
    RUN_TEST(testAtoiBaseInvalidBase);
    RUN_TEST(testAtoiBaseStopsAtNonBaseChar);
    RUN_TEST(testAtoiBaseDenseRoundTrip);
    RUN_TEST(testAtoiBaseCleanSweep);
}
