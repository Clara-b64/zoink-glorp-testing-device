/*
** c03_ex05 -- ft_strlcat: unsigned int ft_strlcat(char *dest, char *src,
**                                                  unsigned int size);
** Must behave exactly like libc/BSD strlcat:
**   - dlen = strnlen(dest, size)  (dest length, capped at size).
**   - if dlen == size (dest not terminated within size): append nothing and
**     return size + strlen(src).
**   - otherwise append at most size - dlen - 1 bytes of src, always write a
**     terminating '\0', and return dlen + strlen(src).
** The return value is the length strlcat TRIED to build (so >= size means it
** was truncated). It does NOT zero-pad.
**
** strlcat is a BSD function only present in glibc >= 2.38, so rather than
** depend on the system symbol we compare against a portable reference that
** encodes the exact documented behaviour (which glibc's strlcat matches).
*/

#include "runner.h"
#include <string.h>

#define LCAT_BUFSIZE 4096

extern unsigned int	ft_strlcat(char *dest, char *src, unsigned int size);

/* Canonical BSD strlcat. */
static unsigned int	refStrlcat(char *dst, char *src, unsigned int size)
{
    unsigned int	dlen;
    unsigned int	slen;
    unsigned int	i;

    dlen = 0;
    while (dlen < size && dst[dlen] != '\0')
        dlen++;
    slen = 0;
    while (src[slen] != '\0')
        slen++;
    if (dlen == size)
        return (size + slen);
    i = 0;
    while (src[i] != '\0' && dlen + i < size - 1)
    {
        dst[dlen + i] = src[i];
        i++;
    }
    dst[dlen + i] = '\0';
    return (dlen + slen);
}

/* Runs ft_strlcat and the reference on parallel sentinel-filled buffers seeded
** with `destInit`, and asserts both the return value and the WHOLE buffer match
** (so content, terminator, the no-padding rule and the untouched tail are all
** checked). */
static void	assertStrlcat(char *destInit, char *src, unsigned int size)
{
    char			mine[LCAT_BUFSIZE];
    char			ref[LCAT_BUFSIZE];
    unsigned int	rmine;
    unsigned int	rref;
    int				dl;

    dl = (int)strlen(destInit);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, destInit, dl + 1);
    memcpy(ref, destInit, dl + 1);
    rmine = ft_strlcat(mine, src, size);
    rref = refStrlcat(ref, src, size);
    TEST_ASSERT_EQUAL_UINT(rref, rmine);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Plenty of room: full append, return = dlen + slen. */
static void	testStrlcatFullAppend(void)
{
    assertStrlcat("foo", "bar", 20);
    assertStrlcat("This is", "test", 10);
}

/* Not enough room: src is truncated but return is still dlen + slen. */
static void	testStrlcatTruncated(void)
{
    assertStrlcat("foo", "bar", 5);
    assertStrlcat("Hello, ", "World!", 10);
}

/* size 0: nothing written, returns strlen(src). */
static void	testStrlcatZeroSize(void)
{
    assertStrlcat("abc", "def", 0);
    assertStrlcat("", "def", 0);
}

/* size <= strlen(dest): the special branch -- return size + strlen(src), no
** copy, dest untouched. */
static void	testStrlcatSizeSmallerThanDest(void)
{
    assertStrlcat("hello", "world", 3);
    assertStrlcat("hello", "world", 5);
    assertStrlcat("hello", "world", 1);
}

/* size == strlen(dest) + 1: only room for the terminator, no src appended. */
static void	testStrlcatRoomForTerminatorOnly(void)
{
    assertStrlcat("hello", "world", 6);
}

/* Empty dest behaves like strlcpy. */
static void	testStrlcatEmptyDest(void)
{
    assertStrlcat("", "abc", 10);
    assertStrlcat("", "abc", 2);
    assertStrlcat("", "abc", 1);
}

/* Empty src: nothing appended, return = dlen (capped at size). */
static void	testStrlcatEmptySrc(void)
{
    assertStrlcat("abc", "", 10);
    assertStrlcat("abc", "", 2);
    assertStrlcat("abc", "", 0);
}

/* Both empty. */
static void	testStrlcatBothEmpty(void)
{
    assertStrlcat("", "", 10);
    assertStrlcat("", "", 0);
}

/* The return value (dlen + slen, the truncation marker) is verified by
** assertStrlcat across the full range of sizes, including size < dest, the
** exact-fit boundary, and sizes far larger than needed. */
static void	testStrlcatReturnValue(void)
{
    unsigned int	size;

    size = 0;
    while (size <= 20)
    {
        assertStrlcat("abc", "WXYZ", size);
        size++;
    }
}

/* No zero-padding: bytes past the terminator (within size) are untouched. */
static void	testStrlcatNoPadding(void)
{
    char	dest[16];

    memset(dest, 'X', sizeof(dest));
    memcpy(dest, "ab", 3);
    ft_strlcat(dest, "c", 10);
    TEST_ASSERT_EQUAL_MEMORY("abc", dest, 4);
    TEST_ASSERT_EQUAL_CHAR('X', dest[4]);
}

/* High bytes are copied verbatim. */
static void	testStrlcatHighBytes(void)
{
    assertStrlcat("a\x80", "\xff\x01z", 10);
    assertStrlcat("a\x80", "\xff\x01z", 3);
}

/* Exhaustive over small dest/src lengths and every size up to dl+sl+3. */
static void	testStrlcatExhaustive(void)
{
    char			dest[16];
    char			src[16];
    int				dl;
    int				sl;
    int				i;
    unsigned int	size;

    dl = 0;
    while (dl <= 6)
    {
        sl = 0;
        while (sl <= 6)
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
            size = 0;
            while (size <= (unsigned int)(dl + sl + 3))
            {
                assertStrlcat(dest, src, size);
                size++;
            }
            sl++;
        }
        dl++;
    }
}

/* A long append, both with and without room. */
static void	testStrlcatLong(void)
{
    char	dest[1001];
    char	src[1001];
    int		i;

    i = 0;
    while (i < 500)
    {
        dest[i] = (char)('a' + (i % 26));
        i++;
    }
    dest[500] = '\0';
    i = 0;
    while (i < 1000)
    {
        src[i] = (char)('0' + (i % 10));
        i++;
    }
    src[1000] = '\0';
    assertStrlcat(dest, src, 2000);
    assertStrlcat(dest, src, 800);
    assertStrlcat(dest, src, 100);
}

void	glorpRun(void)
{
    RUN_TEST(testStrlcatFullAppend);
    RUN_TEST(testStrlcatTruncated);
    RUN_TEST(testStrlcatZeroSize);
    RUN_TEST(testStrlcatSizeSmallerThanDest);
    RUN_TEST(testStrlcatRoomForTerminatorOnly);
    RUN_TEST(testStrlcatEmptyDest);
    RUN_TEST(testStrlcatEmptySrc);
    RUN_TEST(testStrlcatBothEmpty);
    RUN_TEST(testStrlcatReturnValue);
    RUN_TEST(testStrlcatNoPadding);
    RUN_TEST(testStrlcatHighBytes);
    RUN_TEST(testStrlcatExhaustive);
    RUN_TEST(testStrlcatLong);
}
