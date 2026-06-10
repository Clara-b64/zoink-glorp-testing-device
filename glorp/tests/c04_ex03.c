/*
** c04_ex03 -- ft_atoi: int ft_atoi(char *str);
** Parses a leading integer the way the reference does (and the way libc atoi
** does, EXCEPT for multiple signs):
**   1. skip whitespace: ' ' '\t' '\n' '\v' '\f' '\r'
**   2. consume a run of '+'/'-' signs; each '-' flips the sign (so "--5" -> 5)
**   3. read decimal digits until the first non-digit
**   4. return value * sign; no digits -> 0
**
** The oracle refAtoi replicates this, but accumulates in `long` so the test
** harness itself never relies on signed overflow. Inputs are kept within the
** int range -- values that overflow int are undefined behaviour for atoi and
** are deliberately not asserted.
*/

#include "runner.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

extern int	ft_atoi(char *str);

static int	refIsspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n'
        || c == '\v' || c == '\f' || c == '\r');
}

/* Reference atoi (counts multiple signs), accumulating in long to stay UB-free
** for in-range results. */
static int	refAtoi(char *s)
{
    int		i;
    long	result;
    int		sign;

    i = 0;
    result = 0;
    sign = 1;
    while (refIsspace(s[i]))
        i++;
    while (s[i] == '+' || s[i] == '-')
    {
        if (s[i] == '-')
            sign = -sign;
        i++;
    }
    while (s[i] >= '0' && s[i] <= '9')
    {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    return ((int)(result * sign));
}

/* Asserts ft_atoi matches the reference parse for the given string. */
static void	assertAtoi(char *s)
{
    TEST_ASSERT_EQUAL_INT(refAtoi(s), ft_atoi(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Plain positive and negative numbers. */
static void	testAtoiBasic(void)
{
    TEST_ASSERT_EQUAL_INT(42, ft_atoi("42"));
    TEST_ASSERT_EQUAL_INT(-42, ft_atoi("-42"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("0"));
    assertAtoi("7");
    assertAtoi("123456");
    assertAtoi("-987654");
}

/* Leading whitespace is skipped: each whitespace char, and mixes of them. */
static void	testAtoiLeadingWhitespace(void)
{
    assertAtoi(" 42");
    assertAtoi("\t42");
    assertAtoi("\n42");
    assertAtoi("\v42");
    assertAtoi("\f42");
    assertAtoi("\r42");
    assertAtoi("   \t\n\v\f\r  42");
    assertAtoi(" \t -42");
}

/* A single sign. */
static void	testAtoiSingleSign(void)
{
    assertAtoi("+42");
    assertAtoi("-42");
    assertAtoi("+0");
    assertAtoi("-0");
}

/* Multiple signs are counted (the chosen contract): each '-' flips the sign. */
static void	testAtoiMultipleSigns(void)
{
    TEST_ASSERT_EQUAL_INT(5, ft_atoi("--5"));
    TEST_ASSERT_EQUAL_INT(-5, ft_atoi("---5"));
    TEST_ASSERT_EQUAL_INT(-5, ft_atoi("+-5"));
    TEST_ASSERT_EQUAL_INT(-5, ft_atoi("-+5"));
    TEST_ASSERT_EQUAL_INT(5, ft_atoi("++5"));
    TEST_ASSERT_EQUAL_INT(5, ft_atoi("+-+-+5"));
    assertAtoi("-----1");
    assertAtoi("------1");
}

/* Parsing stops at the first non-digit. */
static void	testAtoiStopsAtNonDigit(void)
{
    assertAtoi("42abc");
    assertAtoi("123 456");
    assertAtoi("3.14");
    assertAtoi("0x1F");
    assertAtoi("12-34");
    assertAtoi("7+7");
}

/* Whitespace is NOT skipped after the sign or between digits. */
static void	testAtoiNoWhitespaceAfterSign(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("- 5"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("+ 5"));
    assertAtoi("-\t5");
    assertAtoi("1 2");
    assertAtoi("12 34");
}

/* Leading zeros are ignored in value. */
static void	testAtoiLeadingZeros(void)
{
    assertAtoi("007");
    assertAtoi("000");
    assertAtoi("-0042");
    assertAtoi("0000000000000000000000005");
}

/* No digits at all yields 0. */
static void	testAtoiNoDigits(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_atoi(""));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("abc"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("   "));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("+"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("-"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("+-"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("."));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("z123"));
    TEST_ASSERT_EQUAL_INT(0, ft_atoi("\x1f"));
}

/* The int boundaries (well-defined for atoi). */
static void	testAtoiIntBoundaries(void)
{
    TEST_ASSERT_EQUAL_INT(2147483647, ft_atoi("2147483647"));
    TEST_ASSERT_EQUAL_INT(INT_MIN, ft_atoi("-2147483648"));
    TEST_ASSERT_EQUAL_INT(2147483646, ft_atoi("2147483646"));
    TEST_ASSERT_EQUAL_INT(-2147483647, ft_atoi("-2147483647"));
    assertAtoi("2147483647");
    assertAtoi("-2147483648");
    assertAtoi("1000000000");
    assertAtoi("-1000000000");
}

/* Trailing whitespace/junk after the number is ignored. */
static void	testAtoiTrailingJunk(void)
{
    assertAtoi("42   ");
    assertAtoi("  42  abc");
    assertAtoi("-13!");
    assertAtoi("+99\n");
}

/* A dense sweep of small values both signs. */
static void	testAtoiDenseRange(void)
{
    char	buf[16];
    int		n;

    n = -2000;
    while (n <= 2000)
    {
        snprintf(buf, sizeof(buf), "%d", n);
        assertAtoi(buf);
        n++;
    }
}

/* Combinatorial sweep: whitespace x signs x digit-body x trailing-junk, all
** compared against the reference parse. */
static void	testAtoiCombinatorialSweep(void)
{
    char	*ws[6];
    char	*signs[9];
    char	*bodies[9];
    char	*junk[7];
    char	buf[64];
    int		a;
    int		b;
    int		c;
    int		d;

    ws[0] = "";
    ws[1] = " ";
    ws[2] = "\t";
    ws[3] = "\n";
    ws[4] = "\v\f\r";
    ws[5] = "  \t \n ";
    signs[0] = "";
    signs[1] = "+";
    signs[2] = "-";
    signs[3] = "++";
    signs[4] = "--";
    signs[5] = "+-";
    signs[6] = "-+";
    signs[7] = "---";
    signs[8] = "+-+-+";
    bodies[0] = "0";
    bodies[1] = "00";
    bodies[2] = "5";
    bodies[3] = "42";
    bodies[4] = "00042";
    bodies[5] = "123";
    bodies[6] = "999";
    bodies[7] = "1000000000";
    bodies[8] = "2147483647";
    junk[0] = "";
    junk[1] = "abc";
    junk[2] = " 5";
    junk[3] = ".5";
    junk[4] = "+3";
    junk[5] = "z9";
    junk[6] = "\t";
    a = 0;
    while (a < 6)
    {
        b = 0;
        while (b < 9)
        {
            c = 0;
            while (c < 9)
            {
                d = 0;
                while (d < 7)
                {
                    buf[0] = '\0';
                    strcat(buf, ws[a]);
                    strcat(buf, signs[b]);
                    strcat(buf, bodies[c]);
                    strcat(buf, junk[d]);
                    assertAtoi(buf);
                    d++;
                }
                c++;
            }
            b++;
        }
        a++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testAtoiBasic);
    RUN_TEST(testAtoiLeadingWhitespace);
    RUN_TEST(testAtoiSingleSign);
    RUN_TEST(testAtoiMultipleSigns);
    RUN_TEST(testAtoiStopsAtNonDigit);
    RUN_TEST(testAtoiNoWhitespaceAfterSign);
    RUN_TEST(testAtoiLeadingZeros);
    RUN_TEST(testAtoiNoDigits);
    RUN_TEST(testAtoiIntBoundaries);
    RUN_TEST(testAtoiTrailingJunk);
    RUN_TEST(testAtoiDenseRange);
    RUN_TEST(testAtoiCombinatorialSweep);
}
