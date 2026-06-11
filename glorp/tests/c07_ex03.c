/*
** c07_ex03 -- ft_strjoin: char *ft_strjoin(int size, char **strs, char *sep);
** Joins the `size` strings of strs into one malloc'd string with `sep` placed
** BETWEEN consecutive strings (size - 1 separators, none leading or trailing).
** size == 0 returns a fresh, free-able empty string "" (strs is not read).
**
** Most cases compare ft_strjoin against a reference join of the same inputs, so
** any wrong CONTENT (trailing/leading separator, dropped separator, missing
** terminator, only first sep char, modified inputs, NULL on size 0, ...) fails.
**
** A correct ft_strjoin must allocate exactly Sum(strlen(strs[i])) +
** strlen(sep) * (size - 1) + 1 bytes. Allocating too few still produces the
** right bytes (the overflow lands in malloc slack), so it is invisible to a
** content check -- it only shows up as a heap-buffer-overflow under
** AddressSanitizer. testStrjoinShortSeparatorsManySizes deliberately drives
** every size with 0- and 1-char separators (the inputs that make an off-by-N
** size formula write past the buffer) so an ASan build catches it.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

extern char	*ft_strjoin(int size, char **strs, char *sep);

/* Builds the expected join (strs[0] sep strs[1] sep ... strs[size-1]) into out. */
static void	refJoin(int size, char **strs, char *sep, char *out)
{
    int		i;

    out[0] = '\0';
    i = 0;
    while (i < size)
    {
        strcat(out, strs[i]);
        if (i < size - 1)
            strcat(out, sep);
        i++;
    }
}

/* Joins via ft_strjoin and asserts it equals the reference (content AND exact
** length, so a missing terminator or stray byte fails too), then frees. */
static void	assertJoin(int size, char **strs, char *sep)
{
    char	*result;
    char	expected[2048];

    refJoin(size, strs, sep, expected);
    result = ft_strjoin(size, strs, sep);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);
    TEST_ASSERT_EQUAL_INT((int)strlen(expected), (int)strlen(result));
    free(result);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* size 0 yields a fresh, free-able empty string, ignoring strs. */
static void	testStrjoinSizeZeroReturnsEmptyString(void)
{
    char	*strs[] = {"ignored", "also ignored"};
    char	*result;

    result = ft_strjoin(0, strs, " ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);
    free(result);
}

/* size 0 must not dereference strs at all (NULL strs is fine). */
static void	testStrjoinSizeZeroIgnoresStrs(void)
{
    char	*result;

    result = ft_strjoin(0, NULL, "-");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);
    free(result);
}

/* size 1: just a copy of the one string; the separator is never inserted. */
static void	testStrjoinSingleString(void)
{
    char	*strs[] = {"solo"};

    assertJoin(1, strs, "");
    assertJoin(1, strs, " ");
    assertJoin(1, strs, "--");
    assertJoin(1, strs, "long-separator");
}

/* size 1 with an empty string is the empty string. */
static void	testStrjoinSingleEmptyString(void)
{
    char	*strs[] = {""};

    assertJoin(1, strs, "x");
    assertJoin(1, strs, "");
}

/* Two strings, one separator between them. */
static void	testStrjoinTwoStrings(void)
{
    char	*strs[] = {"Hello", "World"};

    assertJoin(2, strs, " ");
    assertJoin(2, strs, "");
    assertJoin(2, strs, "___");
}

/* The separator goes only BETWEEN strings: no leading, no trailing one. */
static void	testStrjoinSeparatorBetweenNotAround(void)
{
    char	*strs[] = {"a", "b", "c"};
    char	*result;

    result = ft_strjoin(3, strs, "|");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("a|b|c", result);
    TEST_ASSERT_EQUAL_CHAR('a', result[0]);
    TEST_ASSERT_EQUAL_CHAR('c', result[4]);
    TEST_ASSERT_EQUAL_CHAR('\0', result[5]);
    free(result);
}

/* Several strings with various separators. */
static void	testStrjoinManyStrings(void)
{
    char	*strs[] = {"a", "b", "c", "d", "e"};

    assertJoin(5, strs, "");
    assertJoin(5, strs, "-");
    assertJoin(5, strs, ", ");
}

/* Empty members still get separators around them (leading/middle/trailing). */
static void	testStrjoinEmptyStringsInArray(void)
{
    char	*strs[] = {"", "x", "", "y", ""};
    char	*allEmpty[] = {"", "", ""};

    assertJoin(5, strs, "-");
    assertJoin(5, strs, "");
    assertJoin(3, allEmpty, "+");
    assertJoin(3, allEmpty, "");
}

/* Multi-character separator between every pair. */
static void	testStrjoinMultiCharSeparator(void)
{
    char	*strs[] = {"output", "said", "this", "is", "a", "success"};

    assertJoin(6, strs, "___");
}

/* Separators and strings containing control and punctuation characters. */
static void	testStrjoinSpecialChars(void)
{
    char	*lines[] = {"line1", "line2", "line3"};
    char	*digits[] = {"12", "34", "56"};
    char	*punct[] = {"a.b", "c,d", "e;f"};

    assertJoin(3, lines, "\n");
    assertJoin(3, lines, "\t");
    assertJoin(3, digits, " + ");
    assertJoin(3, punct, "::");
}

/* A longer overall result. */
static void	testStrjoinLongResult(void)
{
    char	*strs[] = {"the", "quick", "brown", "fox",
        "jumps", "over", "the", "lazy", "dog"};

    assertJoin(9, strs, " ");
    assertJoin(9, strs, "");
}

/* Every size 1..20 with 0- and 1-char separators: the exact inputs that make an
** undersized allocation write past the buffer (caught under AddressSanitizer). */
static void	testStrjoinShortSeparatorsManySizes(void)
{
    char	*strs[20];
    int		n;
    int		i;

    i = 0;
    while (i < 20)
    {
        strs[i] = "word";
        i++;
    }
    n = 1;
    while (n <= 20)
    {
        assertJoin(n, strs, "");
        assertJoin(n, strs, "-");
        n++;
    }
}

/* ft_strjoin must not modify the input strings or the separator. */
static void	testStrjoinDoesNotModifyInputs(void)
{
    char	a[] = "alpha";
    char	b[] = "beta";
    char	sep[] = "--";
    char	*strs[2];
    char	*result;

    strs[0] = a;
    strs[1] = b;
    result = ft_strjoin(2, strs, sep);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("alpha--beta", result);
    TEST_ASSERT_EQUAL_STRING("alpha", a);
    TEST_ASSERT_EQUAL_STRING("beta", b);
    TEST_ASSERT_EQUAL_STRING("--", sep);
    free(result);
}

/* Each call returns its own allocation. */
static void	testStrjoinReturnsFreshAllocations(void)
{
    char	*strs[] = {"a", "b"};
    char	*x;
    char	*y;

    x = ft_strjoin(2, strs, "-");
    y = ft_strjoin(2, strs, "-");
    TEST_ASSERT_NOT_NULL(x);
    TEST_ASSERT_NOT_NULL(y);
    TEST_ASSERT_TRUE(x != y);
    TEST_ASSERT_EQUAL_STRING("a-b", x);
    TEST_ASSERT_EQUAL_STRING("a-b", y);
    free(x);
    free(y);
}

void	glorpRun(void)
{
    RUN_TEST(testStrjoinSizeZeroReturnsEmptyString);
    RUN_TEST(testStrjoinSizeZeroIgnoresStrs);
    RUN_TEST(testStrjoinSingleString);
    RUN_TEST(testStrjoinSingleEmptyString);
    RUN_TEST(testStrjoinTwoStrings);
    RUN_TEST(testStrjoinSeparatorBetweenNotAround);
    RUN_TEST(testStrjoinManyStrings);
    RUN_TEST(testStrjoinEmptyStringsInArray);
    RUN_TEST(testStrjoinMultiCharSeparator);
    RUN_TEST(testStrjoinSpecialChars);
    RUN_TEST(testStrjoinLongResult);
    RUN_TEST(testStrjoinShortSeparatorsManySizes);
    RUN_TEST(testStrjoinDoesNotModifyInputs);
    RUN_TEST(testStrjoinReturnsFreshAllocations);
}
