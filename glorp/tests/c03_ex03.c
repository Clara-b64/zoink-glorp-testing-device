/*
** c03_ex03 -- ft_strncat: char *ft_strncat(char *dest, char *src, unsigned int nb);
** Must behave exactly like libc strncat: append at most nb bytes of src to the
** end of dest, then ALWAYS write a terminating '\0', and return dest. Unlike
** strncpy there is no zero-padding: at most min(nb, strlen(src)) bytes are
** appended, followed by a single terminator.
**
** The helper runs the real strncat on a parallel buffer and compares the WHOLE
** buffers (sentinel-filled), so the appended bytes, the terminator, the
** preserved prefix and the untouched tail are all verified at once.
*/

#include "runner.h"
#include <string.h>

#define NCAT_BUFSIZE 4096

extern char	*ft_strncat(char *dest, char *src, unsigned int nb);

/* Builds dest from `destInit` in two sentinel-filled buffers, appends `src`
** (limited to nb) with ft_strncat and with libc strncat, and asserts
** full-buffer equality plus that ft_strncat returns dest. */
static void	assertStrncat(char *destInit, char *src, unsigned int nb)
{
    char	mine[NCAT_BUFSIZE];
    char	ref[NCAT_BUFSIZE];
    char	*ret;
    int		dl;

    dl = (int)strlen(destInit);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, destInit, dl + 1);
    memcpy(ref, destInit, dl + 1);
    ret = ft_strncat(mine, src, nb);
    strncat(ref, src, nb);
    TEST_ASSERT_EQUAL_PTR(mine, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* nb >= strlen(src): the whole src is appended (like strcat). */
static void	testStrncatFullAppend(void)
{
    assertStrncat("Hello, ", "World!", 6);
    assertStrncat("Hello, ", "World!", 100);
}

/* nb < strlen(src): only the first nb bytes are appended, then a terminator. */
static void	testStrncatTruncatedAppend(void)
{
    assertStrncat("ab", "cdef", 2);
    assertStrncat("ab", "cdef", 3);
}

/* nb == 0 appends nothing and rewrites the terminator in place (dest unchanged). */
static void	testStrncatZeroNb(void)
{
    assertStrncat("abc", "def", 0);
    assertStrncat("", "def", 0);
}

/* There is NO zero-padding: nb larger than src appends only strlen(src) bytes. */
static void	testStrncatNoPadding(void)
{
    char	dest[16];

    memset(dest, 'X', sizeof(dest));
    memcpy(dest, "ab", 3);
    ft_strncat(dest, "cd", 10);
    TEST_ASSERT_EQUAL_MEMORY("abcd", dest, 5);
    TEST_ASSERT_EQUAL_CHAR('X', dest[5]);
}

/* Empty destination. */
static void	testStrncatEmptyDest(void)
{
    assertStrncat("", "abc", 3);
    assertStrncat("", "abc", 1);
}

/* Empty src appends nothing (terminator rewritten). */
static void	testStrncatEmptySrc(void)
{
    assertStrncat("abc", "", 5);
    assertStrncat("abc", "", 0);
}

/* The return value is the destination pointer. */
static void	testStrncatReturnsDest(void)
{
    char	dest[16];
    char	*ret;

    memcpy(dest, "ab", 3);
    ret = ft_strncat(dest, "cd", 2);
    TEST_ASSERT_EQUAL_PTR(dest, ret);
}

/* The original dest content is preserved; only the limited tail is appended. */
static void	testStrncatPreservesDestPrefix(void)
{
    char	dest[16];

    memset(dest, 'X', sizeof(dest));
    memcpy(dest, "abc", 4);
    ft_strncat(dest, "defgh", 2);
    TEST_ASSERT_EQUAL_MEMORY("abcde", dest, 6);
    TEST_ASSERT_EQUAL_CHAR('X', dest[6]);
}

/* Control characters append verbatim. */
static void	testStrncatControlChars(void)
{
    assertStrncat("line\t", "next\nend", 4);
    assertStrncat("line\t", "next\nend", 100);
}

/* High bytes are copied verbatim (strncat does not compare). */
static void	testStrncatHighBytes(void)
{
    assertStrncat("a\x80", "\xff\x01z", 2);
    assertStrncat("a\x80", "\xff\x01z", 100);
}

/* Exhaustive over small dest/src lengths and every nb up to srclen + 2. */
static void	testStrncatExhaustive(void)
{
    char			dest[16];
    char			src[16];
    int				dl;
    int				sl;
    int				i;
    unsigned int	nb;

    dl = 0;
    while (dl <= 8)
    {
        sl = 0;
        while (sl <= 8)
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
            nb = 0;
            while (nb <= (unsigned int)sl + 2)
            {
                assertStrncat(dest, src, nb);
                nb++;
            }
            sl++;
        }
        dl++;
    }
}

/* A long append, both truncated and full. */
static void	testStrncatLong(void)
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
    assertStrncat(dest, src, 500);
    assertStrncat(dest, src, 2000);
}

void	glorpRun(void)
{
    RUN_TEST(testStrncatFullAppend);
    RUN_TEST(testStrncatTruncatedAppend);
    RUN_TEST(testStrncatZeroNb);
    RUN_TEST(testStrncatNoPadding);
    RUN_TEST(testStrncatEmptyDest);
    RUN_TEST(testStrncatEmptySrc);
    RUN_TEST(testStrncatReturnsDest);
    RUN_TEST(testStrncatPreservesDestPrefix);
    RUN_TEST(testStrncatControlChars);
    RUN_TEST(testStrncatHighBytes);
    RUN_TEST(testStrncatExhaustive);
    RUN_TEST(testStrncatLong);
}
