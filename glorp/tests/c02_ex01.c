/*
** c02_ex01 -- ft_strncpy: char *ft_strncpy(char *dest, char *src, unsigned int n);
** Must behave identically to libc strncpy: write EXACTLY n bytes to dest.
**   - if strlen(src) >= n: copy the first n bytes of src, NO terminator added.
**   - if strlen(src) <  n: copy src (with its '\0'), then pad with '\0' up to n.
** Returns dest. Bytes at index >= n are never touched.
**
** The main helper runs the real strncpy on a parallel buffer and compares the
** WHOLE buffers (both pre-filled with a sentinel), which verifies the n copied
** bytes, the zero padding, the absence/presence of a terminator, and that
** nothing at index >= n was written.
*/

#include "runner.h"
#include <string.h>

#define STRNCPY_BUFSIZE 4096

extern char	*ft_strncpy(char *dest, char *src, unsigned int n);

/* Copies src with ft_strncpy and libc strncpy into two sentinel-filled buffers
** and asserts they are byte-for-byte identical over their full length, plus
** that ft_strncpy returns dest. */
static void	assertStrncpy(char *src, unsigned int n)
{
    char	dest[STRNCPY_BUFSIZE];
    char	ref[STRNCPY_BUFSIZE];
    char	*ret;

    memset(dest, 'X', sizeof(dest));
    memset(ref, 'X', sizeof(ref));
    ret = ft_strncpy(dest, src, n);
    strncpy(ref, src, n);
    TEST_ASSERT_EQUAL_PTR(dest, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, dest, sizeof(dest));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* n == strlen(src): all chars copied, but NO terminator is written. */
static void	testStrncpyNoTerminatorWhenNEqualsLen(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strncpy(dest, "abc", 3);
    TEST_ASSERT_EQUAL_MEMORY("abc", dest, 3);
    TEST_ASSERT_EQUAL_CHAR('X', dest[3]);
}

/* n < strlen(src): only the first n chars are copied, no terminator. */
static void	testStrncpyTruncatesWhenNLessThanLen(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strncpy(dest, "Hello", 3);
    TEST_ASSERT_EQUAL_MEMORY("Hel", dest, 3);
    TEST_ASSERT_EQUAL_CHAR('X', dest[3]);
}

/* n > strlen(src): src is copied, then dest is zero-padded up to n bytes. */
static void	testStrncpyPadsWithZerosWhenNGreaterThanLen(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strncpy(dest, "ab", 5);
    TEST_ASSERT_EQUAL_CHAR('a', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('b', dest[1]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[2]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[3]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[4]);
    TEST_ASSERT_EQUAL_CHAR('X', dest[5]);
}

/* n == 0: nothing is written at all, dest stays as it was; returns dest. */
static void	testStrncpyZeroNWritesNothing(void)
{
    char	dest[8];
    char	*ret;
    int		i;

    memset(dest, 'X', sizeof(dest));
    ret = ft_strncpy(dest, "abc", 0);
    TEST_ASSERT_EQUAL_PTR(dest, ret);
    i = 0;
    while (i < 8)
    {
        TEST_ASSERT_EQUAL_CHAR('X', dest[i]);
        i++;
    }
}

/* Empty source with n > 0: dest gets n zero bytes. */
static void	testStrncpyEmptySourcePadsZeros(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strncpy(dest, "", 4);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[1]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[2]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[3]);
    TEST_ASSERT_EQUAL_CHAR('X', dest[4]);
}

/* The return value is the destination pointer itself. */
static void	testStrncpyReturnsDest(void)
{
    char	dest[16];
    char	*ret;

    ret = ft_strncpy(dest, "abc", 3);
    TEST_ASSERT_EQUAL_PTR(dest, ret);
}

/* n one past the length copies src plus exactly one terminator. */
static void	testStrncpyExactFitWithTerminator(void)
{
    assertStrncpy("abc", 4);
}

/* General cases against libc strncpy: exact, truncating and padding. */
static void	testStrncpyMatchesLibcExact(void)
{
    assertStrncpy("Hello, World!", 13);
}

static void	testStrncpyMatchesLibcTruncate(void)
{
    assertStrncpy("Hello, World!", 5);
}

static void	testStrncpyMatchesLibcPad(void)
{
    assertStrncpy("Hello", 10);
}

/* Control characters, both truncating and padding. */
static void	testStrncpyControlChars(void)
{
    assertStrncpy("a\tb\nc\rd", 4);
    assertStrncpy("a\tb\nc\rd", 12);
}

/* Long strings with n smaller, equal and larger than the length. */
static void	testStrncpyLongString(void)
{
    char	src[2001];
    int		i;

    i = 0;
    while (i < 2000)
    {
        src[i] = 'A' + (i % 26);
        i++;
    }
    src[2000] = '\0';
    assertStrncpy(src, 1000);
    assertStrncpy(src, 2000);
    assertStrncpy(src, 2500);
}

/* Compares ft_strncpy to libc strncpy for `src` across every n from 0 through
** strlen(src) + 8 -- i.e. all regimes: n==0, n<len, n==len, n==len+1 (exact
** terminator) and several n>len (padding). */
static void	sweepAgainstLibc(char *src)
{
    unsigned int	len;
    unsigned int	n;

    len = (unsigned int)strlen(src);
    n = 0;
    while (n <= len + 8)
    {
        assertStrncpy(src, n);
        n++;
    }
}

/* Exhaustive: for every source length 0..20 and every n 0..len+5, ft_strncpy
** must match libc strncpy byte-for-byte over the whole buffer. */
static void	testStrncpyExhaustiveLengthAndN(void)
{
    char	src[32];
    int		len;
    int		i;

    len = 0;
    while (len <= 20)
    {
        i = 0;
        while (i < len)
        {
            src[i] = 'a' + (i % 26);
            i++;
        }
        src[len] = '\0';
        sweepAgainstLibc(src);
        len++;
    }
}

/* Bytes 0x80..0xFF must be copied verbatim, exactly like libc (no signedness
** surprises). Swept across all n regimes. */
static void	testStrncpyHighBytesMatchLibc(void)
{
    char	src[51];
    int		i;

    i = 0;
    while (i < 50)
    {
        src[i] = (char)(((i * 7) % 255) + 1);
        i++;
    }
    src[50] = '\0';
    sweepAgainstLibc(src);
}

/* A representative spread of source lengths/content, each swept over all n. */
static void	testStrncpySweepRepresentativeSources(void)
{
    char	wide[65];
    int		i;

    sweepAgainstLibc("");
    sweepAgainstLibc("a");
    sweepAgainstLibc("abc");
    sweepAgainstLibc("Hello, World!");
    i = 0;
    while (i < 64)
    {
        wide[i] = '!' + (i % 90);
        i++;
    }
    wide[64] = '\0';
    sweepAgainstLibc(wide);
}

void	glorpRun(void)
{
    RUN_TEST(testStrncpyNoTerminatorWhenNEqualsLen);
    RUN_TEST(testStrncpyTruncatesWhenNLessThanLen);
    RUN_TEST(testStrncpyPadsWithZerosWhenNGreaterThanLen);
    RUN_TEST(testStrncpyZeroNWritesNothing);
    RUN_TEST(testStrncpyEmptySourcePadsZeros);
    RUN_TEST(testStrncpyReturnsDest);
    RUN_TEST(testStrncpyExactFitWithTerminator);
    RUN_TEST(testStrncpyMatchesLibcExact);
    RUN_TEST(testStrncpyMatchesLibcTruncate);
    RUN_TEST(testStrncpyMatchesLibcPad);
    RUN_TEST(testStrncpyControlChars);
    RUN_TEST(testStrncpyLongString);
    RUN_TEST(testStrncpyExhaustiveLengthAndN);
    RUN_TEST(testStrncpyHighBytesMatchLibc);
    RUN_TEST(testStrncpySweepRepresentativeSources);
}
