/*
** c00_ex01 -- ft_print_alphabet: void ft_print_alphabet(void);
** Writes the lowercase alphabet in order, on one line, to stdout (fd 1).
*/

#include "runner.h"
#include <unistd.h>

extern void	ft_print_alphabet(void);

/* Runs ft_print_alphabet() with fd 1 redirected into a pipe, restores stdout,
** then returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePrintAlphabet(char *out, int cap)
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
    ft_print_alphabet();
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

/* The whole output is exactly the 26-letter lowercase alphabet. */
static void	testPrintAlphabetOutputsTheLowercaseAlphabet(void)
{
    char	out[32];
    int		n;

    n = capturePrintAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    TEST_ASSERT_EQUAL_STRING("abcdefghijklmnopqrstuvwxyz", out);
}

/* Exactly 26 bytes: no trailing newline and no extra characters. */
static void	testPrintAlphabetWritesExactlyTwentySixBytes(void)
{
    char	out[32];
    int		n;

    n = capturePrintAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
}

/* First byte is 'a', last byte is 'z'. */
static void	testPrintAlphabetStartsAtAEndsAtZ(void)
{
    char	out[32];
    int		n;

    n = capturePrintAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    TEST_ASSERT_EQUAL_CHAR('a', out[0]);
    TEST_ASSERT_EQUAL_CHAR('z', out[25]);
}

/* Letters are consecutive and in ascending order. */
static void	testPrintAlphabetIsInOrder(void)
{
    char	out[32];
    int		n;
    int		i;

    n = capturePrintAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    i = 0;
    while (i < 26)
    {
        TEST_ASSERT_EQUAL_CHAR('a' + i, out[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPrintAlphabetOutputsTheLowercaseAlphabet);
    RUN_TEST(testPrintAlphabetWritesExactlyTwentySixBytes);
    RUN_TEST(testPrintAlphabetStartsAtAEndsAtZ);
    RUN_TEST(testPrintAlphabetIsInOrder);
}
