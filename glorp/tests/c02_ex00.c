/*
** c02_ex00 -- ft_strcpy: char *ft_strcpy(char *dest, char *src);
** Must behave identically to libc strcpy: copy every byte of src INCLUDING the
** terminating '\0' into dest, write nothing past that terminator, and return
** dest.
**
** The main helper runs the real strcpy on a parallel buffer and compares the
** WHOLE buffers (both pre-filled with a sentinel), which at once verifies the
** content, the copied terminator, and that no byte beyond it was written.
*/

#include "runner.h"
#include <string.h>

#define STRCPY_BUFSIZE 4096

extern char	*ft_strcpy(char *dest, char *src);

/* Copies src with ft_strcpy and with libc strcpy into two sentinel-filled
** buffers and asserts they are byte-for-byte identical over their full length,
** plus that ft_strcpy returns dest. */
static void	assertStrcpy(char *src)
{
    char	dest[STRCPY_BUFSIZE];
    char	ref[STRCPY_BUFSIZE];
    char	*ret;

    memset(dest, 'X', sizeof(dest));
    memset(ref, 'X', sizeof(ref));
    ret = ft_strcpy(dest, src);
    strcpy(ref, src);
    TEST_ASSERT_EQUAL_PTR(dest, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, dest, sizeof(dest));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain string is copied verbatim. */
static void	testStrcpyBasic(void)
{
    assertStrcpy("Hello, World!");
}

/* The return value is the destination pointer itself. */
static void	testStrcpyReturnsDest(void)
{
    char	dest[16];
    char	*ret;

    ret = ft_strcpy(dest, "abc");
    TEST_ASSERT_EQUAL_PTR(dest, ret);
}

/* An empty source yields just a terminator at dest[0]. */
static void	testStrcpyEmptySource(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strcpy(dest, "");
    TEST_ASSERT_EQUAL_CHAR('\0', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('X', dest[1]);
}

/* The terminator is copied, and the byte just after it is left untouched. */
static void	testStrcpyCopiesTerminatorAndStopsThere(void)
{
    char	dest[8];

    memset(dest, 'X', sizeof(dest));
    ft_strcpy(dest, "abc");
    TEST_ASSERT_EQUAL_CHAR('a', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('b', dest[1]);
    TEST_ASSERT_EQUAL_CHAR('c', dest[2]);
    TEST_ASSERT_EQUAL_CHAR('\0', dest[3]);
    TEST_ASSERT_EQUAL_CHAR('X', dest[4]);
}

/* A single character. */
static void	testStrcpySingleChar(void)
{
    assertStrcpy("Z");
}

/* Spaces, including leading and trailing. */
static void	testStrcpySpaces(void)
{
    assertStrcpy("  spaced  out  ");
}

/* Embedded control characters. */
static void	testStrcpyControlChars(void)
{
    assertStrcpy("tab\tnewline\nreturn\rend");
}

/* Punctuation and digits. */
static void	testStrcpyPunctuationAndDigits(void)
{
    assertStrcpy("0123456789!@#$%^&*()");
}

/* A long string copies in full. */
static void	testStrcpyLongString(void)
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
    assertStrcpy(src);
}

void	glorpRun(void)
{
    RUN_TEST(testStrcpyBasic);
    RUN_TEST(testStrcpyReturnsDest);
    RUN_TEST(testStrcpyEmptySource);
    RUN_TEST(testStrcpyCopiesTerminatorAndStopsThere);
    RUN_TEST(testStrcpySingleChar);
    RUN_TEST(testStrcpySpaces);
    RUN_TEST(testStrcpyControlChars);
    RUN_TEST(testStrcpyPunctuationAndDigits);
    RUN_TEST(testStrcpyLongString);
}
