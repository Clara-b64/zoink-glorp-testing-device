/*
** c03_ex01 -- ft_strncmp: int ft_strncmp(char *s1, char *s2, unsigned int n);
** Must behave exactly like libc strncmp: compare at most n bytes as UNSIGNED
** char and return the difference of the first differing bytes, or 0 if the
** first n bytes are equal (n == 0 always returns 0).
**
** Every assertion compares ft_strncmp against the system strncmp on the same
** inputs. As with ft_strcmp, an implementation that subtracts signed char
** deviates from libc on bytes >= 0x80; the HighBytes tests enforce the libc
** (unsigned) behaviour.
*/

#include "runner.h"
#include <string.h>

extern int	ft_strncmp(char *s1, char *s2, unsigned int n);

/* Asserts ft_strncmp returns exactly what libc strncmp returns. */
static void	assertStrncmp(char *a, char *b, unsigned int n)
{
    TEST_ASSERT_EQUAL_INT(strncmp(a, b, n), ft_strncmp(a, b, n));
}

/* Compares a and b against libc strncmp for every n from 0 to the longer
** length + 3 (covers n inside, at, and beyond both strings). */
static void	sweepStrncmp(char *a, char *b)
{
    unsigned int	n;
    unsigned int	la;
    unsigned int	lb;
    unsigned int	max;

    la = (unsigned int)strlen(a);
    lb = (unsigned int)strlen(b);
    max = la;
    if (lb > max)
        max = lb;
    n = 0;
    while (n <= max + 3)
    {
        assertStrncmp(a, b, n);
        n++;
    }
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* n == 0 always returns 0, even for completely different strings. */
static void	testStrncmpZeroN(void)
{
    assertStrncmp("abc", "xyz", 0);
    assertStrncmp("", "anything", 0);
    assertStrncmp("Hello", "World", 0);
}

/* Equal strings compare as 0 for any n. */
static void	testStrncmpEqual(void)
{
    sweepStrncmp("abc", "abc");
    sweepStrncmp("Hello, World!", "Hello, World!");
    sweepStrncmp("", "");
}

/* A difference within the first n bytes is reported. */
static void	testStrncmpDiffersWithinN(void)
{
    assertStrncmp("abc", "abd", 3);
    assertStrncmp("abd", "abc", 3);
}

/* A difference beyond n is NOT seen: only the first n bytes count. */
static void	testStrncmpDiffersBeyondNIsIgnored(void)
{
    assertStrncmp("abc", "abd", 2);
    assertStrncmp("abXXX", "abYYY", 2);
    assertStrncmp("abcdef", "abcZZZ", 3);
}

/* Prefix: the terminator vs a byte is seen only if n reaches it. */
static void	testStrncmpPrefix(void)
{
    assertStrncmp("abc", "abcd", 3);
    assertStrncmp("abc", "abcd", 4);
    assertStrncmp("abcd", "abc", 4);
}

/* Empty vs non-empty across a range of n. */
static void	testStrncmpEmptyVsNonEmpty(void)
{
    sweepStrncmp("", "a");
    sweepStrncmp("a", "");
    sweepStrncmp("", "hello");
}

/* n larger than both strings behaves like strcmp (stops at terminator). */
static void	testStrncmpHugeN(void)
{
    assertStrncmp("ab", "ab", 1000);
    assertStrncmp("ab", "abc", 1000);
    assertStrncmp("abc", "abd", 1000);
}

/* The reference's own kind of sample inputs, fully swept over n. */
static void	testStrncmpSampleInputs(void)
{
    sweepStrncmp("Test1", "OK");
    sweepStrncmp("Same", "Size");
    sweepStrncmp("Size", "SIze");
    sweepStrncmp("ThanMyself", "ThanMyself");
}

/* Exhaustive over ASCII: every single-byte pair (1..127) with n = 1. */
static void	testStrncmpAllAsciiBytePairsN1(void)
{
    char	a[2];
    char	b[2];
    int		c1;
    int		c2;

    a[1] = '\0';
    b[1] = '\0';
    c1 = 1;
    while (c1 <= 127)
    {
        c2 = 1;
        while (c2 <= 127)
        {
            a[0] = (char)c1;
            b[0] = (char)c2;
            TEST_ASSERT_EQUAL_INT(strncmp(a, b, 1), ft_strncmp(a, b, 1));
            c2++;
        }
        c1++;
    }
}

/* Equal first char, second char decides -- with n = 2 (sees it) and n = 1
** (does not), swept over printable ASCII. */
static void	testStrncmpSecondCharWithNBoundary(void)
{
    char	a[3];
    char	b[3];
    int		c1;
    int		c2;

    a[0] = 'm';
    b[0] = 'm';
    a[2] = '\0';
    b[2] = '\0';
    c1 = 32;
    while (c1 <= 126)
    {
        c2 = 32;
        while (c2 <= 126)
        {
            a[1] = (char)c1;
            b[1] = (char)c2;
            TEST_ASSERT_EQUAL_INT(strncmp(a, b, 1), ft_strncmp(a, b, 1));
            TEST_ASSERT_EQUAL_INT(strncmp(a, b, 2), ft_strncmp(a, b, 2));
            c2++;
        }
        c1++;
    }
}

/* HighBytes: within n, bytes >= 0x80 compare as UNSIGNED char, like libc. */
static void	testStrncmpHighBytesUseUnsignedComparison(void)
{
    assertStrncmp("\x80", "\x01", 1);
    assertStrncmp("\x01", "\x80", 1);
    assertStrncmp("\xff", "\x7f", 1);
    assertStrncmp("\xff", "\xfe", 1);
    assertStrncmp("\x80", "\x80", 1);
    assertStrncmp("abc\x80", "abc\x01", 4);
    assertStrncmp("abc\x01", "abc\x80", 4);
}

/* A high-byte difference past n is ignored, exactly like libc. */
static void	testStrncmpHighByteBeyondNIgnored(void)
{
    assertStrncmp("abc\x80", "abc\x01", 3);
    assertStrncmp("\x80zzz", "\x01zzz", 0);
}

void	glorpRun(void)
{
    RUN_TEST(testStrncmpZeroN);
    RUN_TEST(testStrncmpEqual);
    RUN_TEST(testStrncmpDiffersWithinN);
    RUN_TEST(testStrncmpDiffersBeyondNIsIgnored);
    RUN_TEST(testStrncmpPrefix);
    RUN_TEST(testStrncmpEmptyVsNonEmpty);
    RUN_TEST(testStrncmpHugeN);
    RUN_TEST(testStrncmpSampleInputs);
    RUN_TEST(testStrncmpAllAsciiBytePairsN1);
    RUN_TEST(testStrncmpSecondCharWithNBoundary);
    RUN_TEST(testStrncmpHighBytesUseUnsignedComparison);
    RUN_TEST(testStrncmpHighByteBeyondNIgnored);
}
