/*
** c02_ex10 -- ft_strlcpy: unsigned int ft_strlcpy(char *dest, char *src,
** unsigned int size);
** Behaves like strlcpy: copies at most size - 1 characters from src into dest,
** NUL-terminating dest whenever size > 0, and returns the length of src. When
** size is 0 dest is not written at all. Guard bytes around the destination
** verify nothing is written past the allowed region.
*/

#include "runner.h"
#include <string.h>

extern unsigned int	ft_strlcpy(char *dest, char *src, unsigned int size);

/* Fills dest with sentinels, runs ft_strlcpy and checks the copied prefix, the
** terminator, the untouched guard area and the return value. */
static void	assertStrlcpy(char *src, unsigned int size)
{
    char			dest[64];
    unsigned int	ret;
    unsigned int	copied;
    unsigned int	i;

    memset(dest, '#', sizeof(dest));
    ret = ft_strlcpy(dest, src, size);
    TEST_ASSERT_EQUAL_UINT((unsigned int)strlen(src), ret);
    if (size == 0)
    {
        i = 0;
        while (i < sizeof(dest))
        {
            TEST_ASSERT_EQUAL_CHAR('#', dest[i]);
            i++;
        }
        return ;
    }
    copied = (unsigned int)strlen(src);
    if (copied > size - 1)
        copied = size - 1;
    TEST_ASSERT_EQUAL_INT(0, strncmp(dest, src, copied));
    TEST_ASSERT_EQUAL_CHAR('\0', dest[copied]);
    i = size;
    while (i < sizeof(dest))
    {
        TEST_ASSERT_EQUAL_CHAR('#', dest[i]);
        i++;
    }
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The whole string fits: full copy plus terminator, src length returned. */
static void	testStrlcpyFullCopy(void)
{
    assertStrlcpy("hello", 32);
    assertStrlcpy("hello", 6);
    assertStrlcpy("42", 3);
}

/* size smaller than the string: exactly size - 1 chars plus terminator. */
static void	testStrlcpyTruncates(void)
{
    assertStrlcpy("hello world", 5);
    assertStrlcpy("hello", 2);
    assertStrlcpy("abcdef", 4);
}

/* size 1: only the terminator is written; the length is still returned. */
static void	testStrlcpySizeOne(void)
{
    assertStrlcpy("hello", 1);
    assertStrlcpy("", 1);
}

/* size 0: dest is never written but the src length is still returned. */
static void	testStrlcpySizeZero(void)
{
    assertStrlcpy("hello", 0);
    assertStrlcpy("", 0);
}

/* An empty src is a plain empty copy. */
static void	testStrlcpyEmptySource(void)
{
    assertStrlcpy("", 8);
}

/* The return value is always strlen(src), whatever the truncation. */
static void	testStrlcpyReturnValue(void)
{
    char	dest[64];

    TEST_ASSERT_EQUAL_UINT(11u, ft_strlcpy(dest, "hello world", 64));
    TEST_ASSERT_EQUAL_UINT(11u, ft_strlcpy(dest, "hello world", 3));
    TEST_ASSERT_EQUAL_UINT(0u, ft_strlcpy(dest, "", 64));
}

void	glorpRun(void)
{
    RUN_TEST(testStrlcpyFullCopy);
    RUN_TEST(testStrlcpyTruncates);
    RUN_TEST(testStrlcpySizeOne);
    RUN_TEST(testStrlcpySizeZero);
    RUN_TEST(testStrlcpyEmptySource);
    RUN_TEST(testStrlcpyReturnValue);
}
