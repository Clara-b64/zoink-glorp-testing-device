/*
** c03_ex00 -- ft_strcmp: int ft_strcmp(char *s1, char *s2);
** Must behave exactly like libc strcmp: compare the two strings and return the
** difference of the first differing bytes, where bytes are compared as UNSIGNED
** char (0 if the strings are equal).
**
** Every assertion compares ft_strcmp against the system strcmp on the same
** inputs, so the contract is literally "identical to this machine's libc".
**
** NOTE on high bytes: libc compares as unsigned char, so "\x80" > "\x01". An
** implementation that subtracts signed char (e.g. `return s1[i] - s2[i];`
** without casting to unsigned char) gets the wrong sign once a byte >= 0x80 is
** compared against a byte < 0x80. The "HighBytes" tests below enforce the libc
** (unsigned) behaviour and will fail such an implementation -- that is a real
** deviation from libc, not a test artefact.
*/

#include "runner.h"
#include <string.h>

extern int	ft_strcmp(char *s1, char *s2);

/* Asserts ft_strcmp returns exactly what libc strcmp returns. */
static void	assertStrcmp(char *a, char *b)
{
    TEST_ASSERT_EQUAL_INT(strcmp(a, b), ft_strcmp(a, b));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Equal strings compare as 0. */
static void	testStrcmpEqual(void)
{
    assertStrcmp("abc", "abc");
    assertStrcmp("Hello, World!", "Hello, World!");
}

/* Two empty strings are equal. */
static void	testStrcmpBothEmpty(void)
{
    assertStrcmp("", "");
}

/* Empty vs non-empty in both directions. */
static void	testStrcmpEmptyVsNonEmpty(void)
{
    assertStrcmp("", "a");
    assertStrcmp("a", "");
}

/* Ordering at the first character. */
static void	testStrcmpFirstCharDiffers(void)
{
    assertStrcmp("abc", "bbc");
    assertStrcmp("bbc", "abc");
}

/* Ordering at the last character. */
static void	testStrcmpLastCharDiffers(void)
{
    assertStrcmp("abc", "abd");
    assertStrcmp("abd", "abc");
}

/* Ordering at a middle character. */
static void	testStrcmpMiddleCharDiffers(void)
{
    assertStrcmp("abcde", "abXde");
    assertStrcmp("abXde", "abcde");
}

/* A proper prefix is less than the longer string (terminator vs a byte). */
static void	testStrcmpPrefix(void)
{
    assertStrcmp("abc", "abcd");
    assertStrcmp("abcd", "abc");
    assertStrcmp("", "x");
}

/* Comparison is case sensitive. */
static void	testStrcmpCaseSensitive(void)
{
    assertStrcmp("ABC", "abc");
    assertStrcmp("abc", "ABC");
    assertStrcmp("Size", "SIze");
}

/* The reference's own sample inputs. */
static void	testStrcmpSampleInputs(void)
{
    assertStrcmp("Test1", "OK");
    assertStrcmp("Same", "Size");
    assertStrcmp("ThanMyself", "ThanMyself");
}

/* Single characters in both directions. */
static void	testStrcmpSingleChars(void)
{
    assertStrcmp("a", "b");
    assertStrcmp("b", "a");
    assertStrcmp("a", "a");
}

/* Exhaustive over ASCII: every single-byte pair (1..127) matches libc. */
static void	testStrcmpAllAsciiBytePairs(void)
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
            TEST_ASSERT_EQUAL_INT(strcmp(a, b), ft_strcmp(a, b));
            c2++;
        }
        c1++;
    }
}

/* After an equal first character, the second character decides -- swept over
** printable ASCII to confirm the comparison advances past equal prefixes. */
static void	testStrcmpSecondCharAfterEqualPrefix(void)
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
            TEST_ASSERT_EQUAL_INT(strcmp(a, b), ft_strcmp(a, b));
            c2++;
        }
        c1++;
    }
}

/* Long strings: equal, and differing at several positions. */
static void	testStrcmpLongStrings(void)
{
    char	a[513];
    char	b[513];
    int		i;
    int		pos;

    i = 0;
    while (i < 512)
    {
        a[i] = (char)('!' + (i % 90));
        b[i] = a[i];
        i++;
    }
    a[512] = '\0';
    b[512] = '\0';
    assertStrcmp(a, b);
    pos = 0;
    while (pos < 512)
    {
        b[pos] = (char)(a[pos] + 1);
        assertStrcmp(a, b);
        assertStrcmp(b, a);
        b[pos] = a[pos];
        pos += 37;
    }
}

/* HighBytes: bytes >= 0x80 must be compared as UNSIGNED char, like libc. A
** high byte is greater than any byte < 0x80. */
static void	testStrcmpHighBytesUseUnsignedComparison(void)
{
    assertStrcmp("\x80", "\x01");
    assertStrcmp("\x01", "\x80");
    assertStrcmp("\xff", "\x7f");
    assertStrcmp("\x7f", "\xff");
    assertStrcmp("\xff", "\xfe");
    assertStrcmp("\x80", "\x80");
    assertStrcmp("abc\x80", "abc\x01");
    assertStrcmp("abc\x01", "abc\x80");
}

/* HighBytes vs the terminator: a high byte is greater than '\0'. */
static void	testStrcmpHighByteVsTerminator(void)
{
    assertStrcmp("\x80", "");
    assertStrcmp("", "\x80");
    assertStrcmp("abc", "abc\x80");
    assertStrcmp("abc\x80", "abc");
}

void	glorpRun(void)
{
    RUN_TEST(testStrcmpEqual);
    RUN_TEST(testStrcmpBothEmpty);
    RUN_TEST(testStrcmpEmptyVsNonEmpty);
    RUN_TEST(testStrcmpFirstCharDiffers);
    RUN_TEST(testStrcmpLastCharDiffers);
    RUN_TEST(testStrcmpMiddleCharDiffers);
    RUN_TEST(testStrcmpPrefix);
    RUN_TEST(testStrcmpCaseSensitive);
    RUN_TEST(testStrcmpSampleInputs);
    RUN_TEST(testStrcmpSingleChars);
    RUN_TEST(testStrcmpAllAsciiBytePairs);
    RUN_TEST(testStrcmpSecondCharAfterEqualPrefix);
    RUN_TEST(testStrcmpLongStrings);
    RUN_TEST(testStrcmpHighBytesUseUnsignedComparison);
    RUN_TEST(testStrcmpHighByteVsTerminator);
}
