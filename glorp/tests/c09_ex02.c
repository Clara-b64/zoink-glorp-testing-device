/*
** c09_ex02 -- ft_split: char **ft_split(char *str, char *charset);
** Same subject as c07_ex05: splits str into words, where any character of
** charset is a separator, returning a malloc'd NULL-terminated array of
** malloc'd words with no empty words. The test re-tokenizes str itself and
** asserts the result matches exactly, then frees everything so leak detection
** stays meaningful.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

extern char	**ft_split(char *str, char *charset);

static int	isSep(char c, char *charset)
{
    int	i;

    i = 0;
    while (charset[i] != '\0')
    {
        if (charset[i] == c)
            return (1);
        i++;
    }
    return (0);
}

/* Re-tokenizes str the same way and asserts ft_split produced exactly those
** words, NULL-terminated and non-empty, then frees the whole result. */
static void	assertSplit(char *str, char *charset)
{
    char	**result;
    int		i;
    int		k;
    int		start;
    int		wlen;

    result = ft_split(str, charset);
    TEST_ASSERT_NOT_NULL(result);
    i = 0;
    k = 0;
    while (str[i] != '\0')
    {
        if (isSep(str[i], charset))
        {
            i++;
            continue ;
        }
        start = i;
        while (str[i] != '\0' && !isSep(str[i], charset))
            i++;
        wlen = i - start;
        TEST_ASSERT_NOT_NULL(result[k]);
        TEST_ASSERT_TRUE(wlen > 0);
        TEST_ASSERT_EQUAL_INT(wlen, (int)strlen(result[k]));
        TEST_ASSERT_EQUAL_INT(0, strncmp(result[k], str + start, (size_t)wlen));
        k++;
    }
    TEST_ASSERT_NULL(result[k]);
    i = 0;
    while (i < k)
    {
        free(result[i]);
        i++;
    }
    free(result);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain space-separated sentence. */
static void	testSplitBasic(void)
{
    assertSplit("hello world", " ");
    assertSplit("one two three four", " ");
}

/* A single word produces a one-element (plus terminator) array. */
static void	testSplitSingleWord(void)
{
    assertSplit("hello", " ");
    assertSplit("x", " ");
}

/* Leading, trailing and repeated separators never yield empty words. */
static void	testSplitSurroundingSeparators(void)
{
    assertSplit("   hello   world   ", " ");
    assertSplit(" leading", " ");
    assertSplit("trailing ", " ");
    assertSplit("a,,,b,,c", ",");
}

/* Any character of charset separates. */
static void	testSplitMultipleSeparatorChars(void)
{
    assertSplit("a,b.c;d", ",.;");
    assertSplit("Words to.be/splitted*in@six", "./*@! ");
    assertSplit("..a..b..", ".");
}

/* An empty charset means no separators: the whole string is one word. */
static void	testSplitEmptyCharset(void)
{
    assertSplit("hello world", "");
    assertSplit("nosplit", "");
}

/* Words made of assorted printable characters. */
static void	testSplitSpecialChars(void)
{
    assertSplit("12+34+56", "+");
    assertSplit("a1b2c3-d4e5-f6", "-");
    assertSplit("tab\tnewline\nspace mix", " \t\n");
}

/* No words: an empty string or an all-separator string gives just {NULL}. */
static void	testSplitNoWords(void)
{
    char	**a;
    char	**b;

    a = ft_split("", " ");
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NULL(a[0]);
    free(a);
    b = ft_split("!!!!", "!");
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_NULL(b[0]);
    free(b);
}

/* The exact word count and terminator placement. */
static void	testSplitExactShape(void)
{
    char	**r;

    r = ft_split("  a b  c ", " ");
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(r[0]);
    TEST_ASSERT_EQUAL_STRING("a", r[0]);
    TEST_ASSERT_EQUAL_STRING("b", r[1]);
    TEST_ASSERT_EQUAL_STRING("c", r[2]);
    TEST_ASSERT_NULL(r[3]);
    free(r[0]);
    free(r[1]);
    free(r[2]);
    free(r);
}

/* ft_split must not modify the input string. */
static void	testSplitDoesNotModifyInput(void)
{
    char	str[] = "  hello,world  foo ";
    char	copy[] = "  hello,world  foo ";
    char	**result;
    int		i;

    result = ft_split(str, ", ");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(copy, str);
    i = 0;
    while (result[i] != NULL)
    {
        free(result[i]);
        i++;
    }
    free(result);
}

void	glorpRun(void)
{
    RUN_TEST(testSplitBasic);
    RUN_TEST(testSplitSingleWord);
    RUN_TEST(testSplitSurroundingSeparators);
    RUN_TEST(testSplitMultipleSeparatorChars);
    RUN_TEST(testSplitEmptyCharset);
    RUN_TEST(testSplitSpecialChars);
    RUN_TEST(testSplitNoWords);
    RUN_TEST(testSplitExactShape);
    RUN_TEST(testSplitDoesNotModifyInput);
}
