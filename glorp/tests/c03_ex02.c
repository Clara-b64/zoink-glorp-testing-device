/*
** c03_ex02 -- ft_strcat: char *ft_strcat(char *dest, char *src);
** Must behave exactly like libc strcat: append src (including its '\0') onto the
** end of dest, overwriting dest's old terminator, and return dest. dest's
** existing content is kept and nothing past the new terminator is written.
**
** The helper runs the real strcat on a parallel buffer and compares the WHOLE
** buffers (sentinel-filled), so the appended bytes, the new terminator, the
** preserved prefix, and the untouched tail are all verified at once.
*/

#include "runner.h"
#include <string.h>

#define CAT_BUFSIZE 4096

extern char	*ft_strcat(char *dest, char *src);

/* Builds dest from `destInit` in two sentinel-filled buffers, appends `src`
** with ft_strcat and with libc strcat, and asserts full-buffer equality plus
** that ft_strcat returns dest. */
static void	assertStrcat(char *destInit, char *src)
{
    char	mine[CAT_BUFSIZE];
    char	ref[CAT_BUFSIZE];
    char	*ret;
    int		dl;

    dl = (int)strlen(destInit);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, destInit, dl + 1);
    memcpy(ref, destInit, dl + 1);
    ret = ft_strcat(mine, src);
    strcat(ref, src);
    TEST_ASSERT_EQUAL_PTR(mine, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Appending onto a non-empty destination. */
static void	testStrcatBasic(void)
{
    assertStrcat("Hello, ", "World!");
}

/* Appending onto an empty destination yields just src. */
static void	testStrcatEmptyDest(void)
{
    assertStrcat("", "abc");
}

/* Appending an empty src leaves dest unchanged (terminator rewritten in place). */
static void	testStrcatEmptySrc(void)
{
    assertStrcat("abc", "");
}

/* Both empty. */
static void	testStrcatBothEmpty(void)
{
    assertStrcat("", "");
}

/* The return value is the destination pointer. */
static void	testStrcatReturnsDest(void)
{
    char	dest[16];
    char	*ret;

    memcpy(dest, "ab", 3);
    ret = ft_strcat(dest, "cd");
    TEST_ASSERT_EQUAL_PTR(dest, ret);
}

/* The original dest content is preserved; only the tail is appended. */
static void	testStrcatPreservesDestPrefix(void)
{
    char	dest[16];

    memset(dest, 'X', sizeof(dest));
    memcpy(dest, "abc", 4);
    ft_strcat(dest, "de");
    TEST_ASSERT_EQUAL_MEMORY("abcde", dest, 6);
    TEST_ASSERT_EQUAL_CHAR('X', dest[6]);
}

/* Spaces and punctuation in both parts. */
static void	testStrcatSpacesAndPunctuation(void)
{
    assertStrcat("a b ", "c, d!");
}

/* Control characters append verbatim. */
static void	testStrcatControlChars(void)
{
    assertStrcat("line\t", "next\nend");
}

/* High bytes are copied verbatim (strcat does not compare). */
static void	testStrcatHighBytes(void)
{
    assertStrcat("a\x80", "\xff\x01z");
}

/* Exhaustive over small dest and src lengths against libc strcat. */
static void	testStrcatExhaustiveLengths(void)
{
    char	dest[16];
    char	src[16];
    int		dl;
    int		sl;
    int		i;

    dl = 0;
    while (dl <= 10)
    {
        sl = 0;
        while (sl <= 10)
        {
            i = 0;
            while (i < dl)
            {
                dest[i] = (char)('a' + (i % 26));
                i++;
            }
            dest[dl] = '\0';
            i = 0;
            while (i < sl)
            {
                src[i] = (char)('A' + (i % 26));
                i++;
            }
            src[sl] = '\0';
            assertStrcat(dest, src);
            sl++;
        }
        dl++;
    }
}

/* A long append. */
static void	testStrcatLong(void)
{
    char	dest[1001];
    char	src[1001];
    int		i;

    i = 0;
    while (i < 1000)
    {
        dest[i] = (char)('a' + (i % 26));
        src[i] = (char)('0' + (i % 10));
        i++;
    }
    dest[1000] = '\0';
    src[1000] = '\0';
    assertStrcat(dest, src);
}

void	glorpRun(void)
{
    RUN_TEST(testStrcatBasic);
    RUN_TEST(testStrcatEmptyDest);
    RUN_TEST(testStrcatEmptySrc);
    RUN_TEST(testStrcatBothEmpty);
    RUN_TEST(testStrcatReturnsDest);
    RUN_TEST(testStrcatPreservesDestPrefix);
    RUN_TEST(testStrcatSpacesAndPunctuation);
    RUN_TEST(testStrcatControlChars);
    RUN_TEST(testStrcatHighBytes);
    RUN_TEST(testStrcatExhaustiveLengths);
    RUN_TEST(testStrcatLong);
}
