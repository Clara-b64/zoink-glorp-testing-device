/*
** c00_ex00 -- ft_putchar: void ft_putchar(char c);
** Writes the single byte `c` to stdout (fd 1) and nothing else.
*/

#include "runner.h"
#include <unistd.h>

extern void	ft_putchar(char c);

/* Runs ft_putchar(c) with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on capture failure). */
static int	capturePutchar(char c, char *out, int cap)
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
    ft_putchar(c);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A plain letter comes out unchanged, as a single byte. */
static void	testPutcharOutputsTheGivenLetter(void)
{
    char	out[8];
    int		n;

    n = capturePutchar('A', out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('A', out[0]);
}

/* A digit is a character, not a number. */
static void	testPutcharOutputsADigit(void)
{
    char	out[8];
    int		n;

    n = capturePutchar('7', out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('7', out[0]);
}

/* A control char ('\n') passes through untouched. */
static void	testPutcharOutputsANewline(void)
{
    char	out[8];
    int		n;

    n = capturePutchar('\n', out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('\n', out[0]);
}

/* Exactly one byte: not zero (wrote nothing), not two (wrote extra). */
static void	testPutcharWritesExactlyOneByte(void)
{
    char	out[8];
    int		n;

    n = capturePutchar('z', out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
}

/* The NUL byte is written as one real zero byte. */
static void	testPutcharHandlesNullByte(void)
{
    char	out[8];
    int		n;

    n = capturePutchar('\0', out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('\0', out[0]);
}

/* Every one of the 256 byte values is emitted verbatim. */
static void	testPutcharOutputsFullByteRange(void)
{
    char	out[8];
    int		n;
    int		i;

    i = 0;
    while (i <= 255)
    {
        n = capturePutchar((char)i, out, sizeof(out));
        TEST_ASSERT_EQUAL_INT(1, n);
        TEST_ASSERT_EQUAL_HEX8((unsigned char)i, (unsigned char)out[0]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPutcharOutputsTheGivenLetter);
    RUN_TEST(testPutcharOutputsADigit);
    RUN_TEST(testPutcharOutputsANewline);
    RUN_TEST(testPutcharWritesExactlyOneByte);
    RUN_TEST(testPutcharHandlesNullByte);
    RUN_TEST(testPutcharOutputsFullByteRange);
}
