/*
** c07_ex00 -- ft_strdup: char *ft_strdup(char *src);
** Behaves exactly like libc strdup: malloc(strlen(src) + 1), copy src including
** the terminating '\0', and return the new buffer (NULL only on allocation
** failure). The result is a fresh, independent, free-able heap block -- distinct
** from src, with identical bytes. NULL src is undefined for strdup, so it is not
** tested. Correctness is checked by content (memcmp over len + 1), pointer
** distinctness, mutual independence of the buffers, and that each call returns a
** separately allocated block.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

extern char	*ft_strdup(char *src);

/* Duplicates src and asserts the copy is a distinct, byte-identical, free-able
** heap block (content compared over len + 1 to include the terminator). */
static void	assertStrdup(const char *src)
{
    char	*copy;
    size_t	len;

    len = strlen(src);
    copy = ft_strdup((char *)src);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_TRUE(copy != src);
    TEST_ASSERT_EQUAL_INT(0, memcmp(src, copy, len + 1));
    free(copy);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string yields a 1-byte buffer holding just '\0'. */
static void	testStrdupEmptyString(void)
{
    char	*copy;

    copy = ft_strdup("");
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_CHAR('\0', copy[0]);
    TEST_ASSERT_EQUAL_STRING("", copy);
    free(copy);
}

/* Single-character strings. */
static void	testStrdupSingleChars(void)
{
    assertStrdup("a");
    assertStrdup("Z");
    assertStrdup("5");
    assertStrdup(" ");
}

/* Ordinary strings of varied content. */
static void	testStrdupKnownStrings(void)
{
    assertStrdup("hello");
    assertStrdup("42");
    assertStrdup("Hello, World!");
    assertStrdup("the quick brown fox jumps over the lazy dog");
}

/* The copy is independent of the source in both directions. */
static void	testStrdupIsIndependentCopy(void)
{
    char	src[6];
    char	*copy;

    src[0] = 'h';
    src[1] = 'e';
    src[2] = 'l';
    src[3] = 'l';
    src[4] = 'o';
    src[5] = '\0';
    copy = ft_strdup(src);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_EQUAL_STRING("hello", copy);
    src[0] = 'J';
    TEST_ASSERT_EQUAL_STRING("hello", copy);
    copy[1] = 'a';
    TEST_ASSERT_EQUAL_CHAR('e', src[1]);
    free(copy);
}

/* Each call returns its own allocation, not a shared static buffer. */
static void	testStrdupReturnsFreshAllocations(void)
{
    char	*a;
    char	*b;

    a = ft_strdup("same");
    b = ft_strdup("same");
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_TRUE(a != b);
    TEST_ASSERT_EQUAL_STRING("same", a);
    TEST_ASSERT_EQUAL_STRING("same", b);
    free(a);
    free(b);
}

/* Every non-zero byte value is copied verbatim. */
static void	testStrdupPreservesAllByteValues(void)
{
    char	src[256];
    char	*copy;
    int		i;

    i = 0;
    while (i < 255)
    {
        src[i] = (char)(i + 1);
        i++;
    }
    src[255] = '\0';
    copy = ft_strdup(src);
    TEST_ASSERT_NOT_NULL(copy);
    TEST_ASSERT_TRUE(copy != src);
    TEST_ASSERT_EQUAL_INT(0, memcmp(src, copy, sizeof(src)));
    free(copy);
}

/* Lengths 0..256 are each duplicated exactly. */
static void	testStrdupVariousLengths(void)
{
    char	src[257];
    char	*copy;
    int		len;
    int		i;

    len = 0;
    while (len <= 256)
    {
        i = 0;
        while (i < len)
        {
            src[i] = (char)('A' + i % 26);
            i++;
        }
        src[len] = '\0';
        copy = ft_strdup(src);
        TEST_ASSERT_NOT_NULL(copy);
        TEST_ASSERT_TRUE(copy != src);
        TEST_ASSERT_EQUAL_INT(0, memcmp(src, copy, (size_t)len + 1));
        free(copy);
        len++;
    }
}

/* Actively writes every byte the allocation must hold (data plus the terminator
** slot at index len) across many lengths. With the test archive built under
** AddressSanitizer, an undersized malloc -- e.g. one that forgets the +1 for the
** '\0' -- is caught here as a heap-buffer-overflow instead of slipping by. */
static void	testStrdupBufferIsFullyWritable(void)
{
    char	src[129];
    char	*copy;
    int		len;
    int		i;

    len = 0;
    while (len <= 128)
    {
        i = 0;
        while (i < len)
        {
            src[i] = (char)('a' + i % 26);
            i++;
        }
        src[len] = '\0';
        copy = ft_strdup(src);
        TEST_ASSERT_NOT_NULL(copy);
        i = 0;
        while (i <= len)
        {
            copy[i] = 'Z';
            i++;
        }
        free(copy);
        len++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testStrdupEmptyString);
    RUN_TEST(testStrdupSingleChars);
    RUN_TEST(testStrdupKnownStrings);
    RUN_TEST(testStrdupIsIndependentCopy);
    RUN_TEST(testStrdupReturnsFreshAllocations);
    RUN_TEST(testStrdupPreservesAllByteValues);
    RUN_TEST(testStrdupVariousLengths);
    RUN_TEST(testStrdupBufferIsFullyWritable);
}
