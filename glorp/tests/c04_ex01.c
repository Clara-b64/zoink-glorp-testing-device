/*
** c04_ex01 -- ft_putstr: void ft_putstr(char *str);
** Writes the string str to stdout (fd 1) -- exactly its bytes, no terminating
** '\0', no added newline. (Same contract as c01_ex05.)
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

#define PUTSTR_BUFSIZE 4096

extern void	ft_putstr(char *str);

/* Runs ft_putstr(str) with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePutstr(char *str, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

    saved = dup(STDOUT_FILENO);
    if (saved == -1)
        return (-1);
    if (pipe(pfd) == -1)
    {
        close(saved);
        return (-1);
    }
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    ft_putstr(str);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

/* Captures ft_putstr(str) and asserts the output equals str byte-for-byte,
** including length (so a missing/extra byte or stray newline is caught). */
static void	assertPutstr(char *str)
{
    char	out[PUTSTR_BUFSIZE];
    int		n;
    int		len;

    len = (int)strlen(str);
    n = capturePutstr(str, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(len, n);
    TEST_ASSERT_EQUAL_MEMORY(str, out, len);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain string is written verbatim. */
static void	testPutstrBasic(void)
{
    assertPutstr("Hello, World!");
}

/* An empty string writes nothing at all (not even the '\0'). */
static void	testPutstrEmptyWritesNothing(void)
{
    char	out[PUTSTR_BUFSIZE];
    int		n;

    n = capturePutstr("", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* A single character. */
static void	testPutstrSingleCharacter(void)
{
    assertPutstr("a");
}

/* The terminator is not written: "abc" is 3 bytes, not 4. */
static void	testPutstrDoesNotWriteTerminator(void)
{
    char	out[PUTSTR_BUFSIZE];
    int		n;

    n = capturePutstr("abc", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_MEMORY("abc", out, 3);
}

/* Spaces are preserved, including leading and trailing ones. */
static void	testPutstrPreservesSpaces(void)
{
    assertPutstr("  multiple   spaces  ");
}

/* Embedded control characters pass through untouched. */
static void	testPutstrEmbeddedControlChars(void)
{
    assertPutstr("line1\nline2\ttabbed\rend");
}

/* Punctuation and symbols. */
static void	testPutstrPunctuation(void)
{
    assertPutstr("!@#$%^&*()_+-=[]{}|;:',.<>?/\\\"");
}

/* Digits. */
static void	testPutstrDigits(void)
{
    assertPutstr("0123456789");
}

/* A long string is written in full. */
static void	testPutstrLongString(void)
{
    char	big[2001];
    int		i;

    i = 0;
    while (i < 2000)
    {
        big[i] = 'A' + (i % 26);
        i++;
    }
    big[2000] = '\0';
    assertPutstr(big);
}

void	glorpRun(void)
{
    RUN_TEST(testPutstrBasic);
    RUN_TEST(testPutstrEmptyWritesNothing);
    RUN_TEST(testPutstrSingleCharacter);
    RUN_TEST(testPutstrDoesNotWriteTerminator);
    RUN_TEST(testPutstrPreservesSpaces);
    RUN_TEST(testPutstrEmbeddedControlChars);
    RUN_TEST(testPutstrPunctuation);
    RUN_TEST(testPutstrDigits);
    RUN_TEST(testPutstrLongString);
}
